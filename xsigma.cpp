#include "xsigma.hpp"
#include <format>
#include <iostream>
#include <lzma.h>

#define __debug__

namespace XSigma {

/*
    Default Code Table

        TYPE      SIZE     MODE    TYPE     SIZE     MODE     INDEX
       ---------------------------------------------------------------
    1.  RUN         0        0     NOOP       0        0        0
    2.  ADD    0, [1,17]     0     NOOP       0        0      [1,18]
    3.  COPY   0, [4,18]     0     NOOP       0        0     [19,34]
    4.  COPY   0, [4,18]     1     NOOP       0        0     [35,50]
    5.  COPY   0, [4,18]     2     NOOP       0        0     [51,66]
    6.  COPY   0, [4,18]     3     NOOP       0        0     [67,82]
    7.  COPY   0, [4,18]     4     NOOP       0        0     [83,98]
    8.  COPY   0, [4,18]     5     NOOP       0        0     [99,114]
    9.  COPY   0, [4,18]     6     NOOP       0        0    [115,130]
   10.  COPY   0, [4,18]     7     NOOP       0        0    [131,146]
   11.  COPY   0, [4,18]     8     NOOP       0        0    [147,162]
   12.  ADD       [1,4]      0     COPY     [4,6]      0    [163,174]
   13.  ADD       [1,4]      0     COPY     [4,6]      1    [175,186]
   14.  ADD       [1,4]      0     COPY     [4,6]      2    [187,198]
   15.  ADD       [1,4]      0     COPY     [4,6]      3    [199,210]
   16.  ADD       [1,4]      0     COPY     [4,6]      4    [211,222]
   17.  ADD       [1,4]      0     COPY     [4,6]      5    [223,234]
   18.  ADD       [1,4]      0     COPY       4        6    [235,238]
   19.  ADD       [1,4]      0     COPY       4        7    [239,242]
   20.  ADD       [1,4]      0     COPY       4        8    [243,246]
   21.  COPY        4      [0,8]   ADD        1        0    [247,255]
*/

std::size_t maxIntegerSize { sizeof(uint32_t) };

uint32_t decodeInteger(std::vector<uint8_t>::iterator& src){
    std::size_t bytes = 0;
    uint32_t result = 0;

    do {
        result = (result << 7) | *src & 0x7F;
        src++;
    } while(((*src) & 0x80 && bytes < maxIntegerSize));

    return result;
}

struct Cache {
    uint32_t nextSlot { 0 }; // rings around near 
    std::vector<uint32_t> near {};
    std::vector<uint32_t> same {};

    Cache(){
        near.resize(4, 0);
        same.resize(3*256, 0);
    }

    Cache(uint32_t s_near, uint32_t s_same){
        near.resize(s_near, 0);
        same.resize(s_same*256, 0);
    }

    void Update(uint32_t address){
        if(near.size() > 0){
            near[nextSlot] = address;
            nextSlot = (nextSlot + 1) % near.size();
        }

        if(same.size() > 0){
            same[address % same.size()] = address;
        }
    }

    uint32_t DecodeAddress(std::vector<uint8_t>::iterator buff, uint32_t here, uint32_t mode){
        uint32_t addr, m;
        if(mode == 0) { // VCD_SELF
            addr = decodeInteger(buff);
        } else if(mode == 1) { // VCD_HERE
            addr = here - decodeInteger(buff); // what
        } else if((m = mode - 2) >= 0 && m < near.size()){
            addr = near[m] + decodeInteger(buff);
        } else {
            m = mode - (2 + near.size());
            addr = same[m * 256 + *buff];
            buff++;
        }

        Update(addr);

        return addr;
    }

};


void LZMADecode(std::vector<uint8_t>& data){
    std::vector<uint8_t> decompressed;
    std::vector<uint8_t>::iterator dataIterator = data.begin();
    
    uint32_t decodedSize = decodeInteger(dataIterator);
    
    #ifdef __debug__
    std::cout << "[XSigma] Decompressed Size " << decodedSize << std::endl;
    #endif
    
    decompressed.reserve(decodedSize);


    lzma_stream stream = { nullptr, 0, 0, nullptr, 0, 0, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, 0, 0, 0, 0, LZMA_RESERVED_ENUM, LZMA_RESERVED_ENUM };
    lzma_ret stream_code = lzma_stream_decoder(&stream, UINT64_MAX, LZMA_TELL_NO_CHECK);

    if(stream_code != LZMA_OK){
        std::cout << "Error Setting up lzma stream: " << (uint32_t)stream_code << std::endl;        
    }


    stream.total_in = data.size();
    stream.next_in = &(*dataIterator);

    stream.next_out = decompressed.data();
    stream.total_out = decodedSize;

    lzma_ret code = lzma_code(&stream, LZMA_RUN);

    if(code != LZMA_OK){
        std::cout << "Error Decoding: " << (uint32_t)code << std::endl;
    }

    data = decompressed;
}


// Applies a Patch to a Target
void Decode(std::vector<uint8_t>& target, std::vector<uint8_t>& patch, std::vector<uint8_t>& out){
    // Get some information about patches
    std::vector<uint8_t>::iterator patchReadPtr = patch.begin();

    uint32_t magic;
    uint8_t indicator;
    SigmaCompressionType compressionType;

    uint8_t nearCacheSize, sameCacheSize;
    

    std::memcpy(&magic, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);

    if(magic != 0x00C4C3D6){
        std::cout << std::format("[XSigma] Patch has incorrect magic value (Got 0x{:08X} Expected 0x00C4C3D6)", magic) << std::endl;
        return;
    }
    
    indicator = *patchReadPtr; patchReadPtr++;

    if(indicator & SigmaFlags::Compressed){
        compressionType = static_cast<SigmaCompressionType>(*patchReadPtr); patchReadPtr++;
        
        #ifdef __debug__
        std::cout << std::format("[XSigma] Compression flag is active, Type is 0x{:02X}", static_cast<uint8_t>(compressionType)) << std::endl;
        #endif
    }

    Cache ka();

    if(indicator & SigmaFlags::CodeTable){
        nearCacheSize = *patchReadPtr; patchReadPtr++;
        sameCacheSize = *patchReadPtr; patchReadPtr++;    
    
        patchReadPtr += nearCacheSize + sameCacheSize; // skip internal code table for now

        #ifdef __debug__
        std::cout << std::format("[XSigma] VCDiff has Internal Code Table") << std::endl;
        #endif
    }

    if(indicator & SigmaFlags::ApplicationData){
        #ifdef __debug__
        std::cout << std::format("[XSigma] Application Data flag is active") << std::endl;
        #endif
        uint32_t applDataSize = decodeInteger(patchReadPtr);

        #ifdef __debug__
        std::cout << std::format("[XSigma] Application Size: {}", applDataSize) << std::endl;
        #endif

        std::string data;
        data.resize(applDataSize);
        std::memcpy(data.data(), &(*patchReadPtr), applDataSize); patchReadPtr += applDataSize;
    }

    bool hasRemainingWindows = true;

    while(hasRemainingWindows){
        uint8_t curWindowIndicator = *patchReadPtr; patchReadPtr++;
        uint32_t segmentSize = decodeInteger(patchReadPtr);
        uint32_t segmentPosition = decodeInteger(patchReadPtr);

        #ifdef __debug__
        std::cout << std::format("[XSigma] Window Segment Size {}", segmentSize) << std::endl;
        std::cout << std::format("[XSigma] Window Segment Position {}", segmentPosition) << std::endl;
        #endif

        bool isSourceWindow = (curWindowIndicator & 0x01); // pull from source file
        bool isTargetWindow = (curWindowIndicator & 0x02); // pull from target data

        if(isSourceWindow && isTargetWindow){
            std::cout << std::format("[XSigma] Invalid Window: Window is Source & Target") << std::endl;
            return;
        }

        // Handle delta encoding data
        uint32_t deltaEncodingSize = decodeInteger(patchReadPtr);
        uint32_t targetWindowSize = decodeInteger(patchReadPtr);

        uint8_t deltaIndicator = *patchReadPtr; patchReadPtr++;
        uint32_t addRunDataSize = decodeInteger(patchReadPtr);
        uint32_t instructionsSectionSize = decodeInteger(patchReadPtr);
        uint32_t copyAddrsSize = decodeInteger(patchReadPtr);
    
        std::vector<uint8_t> runData, instructionsSection, copyAddresses;
        runData.resize(addRunDataSize);
        instructionsSection.resize(instructionsSectionSize);
        copyAddresses.resize(copyAddrsSize);

        std::memcpy(runData.data(), &(*patchReadPtr), addRunDataSize); patchReadPtr += addRunDataSize;
        std::memcpy(instructionsSection.data(), &(*patchReadPtr), instructionsSectionSize); patchReadPtr += instructionsSectionSize;
        std::memcpy(copyAddresses.data(), &(*patchReadPtr), copyAddrsSize); patchReadPtr += copyAddrsSize;

        if(deltaIndicator & 0x01){
            // Data for add/run instructions is compressed
            #ifdef __debug__
            std::cout << std::format("[XSigma] Add/Run Data is Compressed") << std::endl;
            #endif

            if(compressionType == XSigma::SigmaCompressionType::LZMA)
                LZMADecode(runData);
            else
                std::cout << std::format("[XSigma] Unsupported Compression Format") << std::endl;
                return;
        }

        if(deltaIndicator & 0x02){
            // Instructions buffer is compressed
            #ifdef __debug__
            std::cout << std::format("[XSigma] Add/Run Instructions Are Compressed") << std::endl;
            #endif

            if(compressionType == XSigma::SigmaCompressionType::LZMA)
                LZMADecode(instructionsSection);
            else
                std::cout << std::format("[XSigma] Unsupported Compression Format") << std::endl;
                return;
        }

        if(deltaIndicator & 0x04){
            // Coppy instructions are compressed
            #ifdef __debug__
            std::cout << std::format("[XSigma] Copy Instructions Are Compressed") << std::endl;
            #endif

            if(compressionType == XSigma::SigmaCompressionType::LZMA)
                LZMADecode(copyAddresses);
            else
                std::cout << std::format("[XSigma] Unsupported Compression Format") << std::endl;
                return;
        }



    }

}

void DecodeBuffer(uint8_t* targetBuffer, std::size_t targetBufferSize, uint8_t* patchBuffer, std::size_t patchBufferSize, uint8_t* outputBuffer, std::size_t& outSize){

}


// vcdif encode
void Encode(std::vector<uint8_t>& source, std::vector<uint8_t>& target, std::vector<uint8_t>& delta, SigmaFlags flags){

    // Create Window Data

    // Write Delta
    delta.reserve(source.size()); // Reserve enough space for source to fit at least
    std::vector<uint8_t>::iterator writePtr = delta.begin();
    
    uint32_t magic = 0xD6C3C400;

    std::memcpy(&(*writePtr), &magic, sizeof(magic));
    writePtr += sizeof(magic);

    uint8_t indicator = 0;
    if(flags & SigmaFlags::Compressed){
        indicator |= 1;
    }

    if(flags & SigmaFlags::CodeTable){
        indicator |= 2;
    }
    
    *writePtr++ = indicator;
    
    if(flags & SigmaFlags::CodeTable){
        uint32_t codeTableSize = 0;
        uint8_t codeTableNearCacheSize = 0;
        uint8_t codeTableSameCacheSize = 0;
        
        std::memcpy(&(*writePtr), &codeTableSize, sizeof(codeTableSize));
        writePtr += sizeof(codeTableSize);
        *writePtr++ = codeTableNearCacheSize;
        *writePtr++ = codeTableSameCacheSize;
    
        //todo: generate code table
    }

    // Write Source Strings Window
    *writePtr++ = 0x01; // Indicator, this is source
    
}

void EncodeBuffer(uint8_t* sourceBuffer, std::size_t soruceBufferSize, uint8_t* targetBuffer, std::size_t targetBufferSize, uint8_t* deltaBuffer, std::size_t& deltaSize){
    std::size_t workingDeltaBufferSize  = soruceBufferSize;
    uint8_t* workingDeltaBuffer = new uint8_t[workingDeltaBufferSize];
    
    // Delta size is the final size of the delta file, the above simply allocates some amount of space for the working buffer;
    // If the delta is going to be larger than the original soure size, why even make a diff/.
    deltaSize = 4;


    delete[] workingDeltaBuffer;
}

}