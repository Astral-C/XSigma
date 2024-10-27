#include "xsigma.hpp"
#include <format>
#include <iostream>
#include <lzma.h>

#define __debug__

namespace XSigma {

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

Cache::Cache(){
    near.resize(4, 0);
    same.resize(3*256, 0);
}

Cache::Cache(uint32_t s_near, uint32_t s_same){
    near.resize(s_near, 0);
    same.resize(s_same*256, 0);
}

void Cache::Update(uint32_t address){
    if(near.size() > 0){
        near[nextSlot] = address;
        nextSlot = (nextSlot + 1) % near.size();
    }

    if(same.size() > 0){
        same[address % same.size()] = address;
    }
}

uint32_t Cache::DecodeAddress(std::vector<uint8_t>::iterator buff, uint32_t here, uint32_t mode){
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

    if(indicator & static_cast<uint8_t>(SigmaFlags::Compressed)){
        compressionType = static_cast<SigmaCompressionType>(*patchReadPtr); patchReadPtr++;
        
        #ifdef __debug__
        std::cout << std::format("[XSigma] Compression flag is active, Type is 0x{:02X}", static_cast<uint8_t>(compressionType)) << std::endl;
        #endif
    }

    Cache ka;
    CodeTable table;

    if(indicator & static_cast<uint8_t>(SigmaFlags::CodeTable)){
        nearCacheSize = *patchReadPtr; patchReadPtr++;
        sameCacheSize = *patchReadPtr; patchReadPtr++;    
    
        patchReadPtr += nearCacheSize + sameCacheSize; // skip internal code table for now

        #ifdef __debug__
        std::cout << std::format("[XSigma] VCDiff has Internal Code Table") << std::endl;
        #endif
    } else {
        table = DefaultCodeTable;
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

        // Make sure our output data has some reserved memory, no idea if it will be enough but it should prevent allocations and, when allocations are needed, allow more mem to be reserved

        // Run the instructions
        std::vector<uint8_t>::iterator addRunPtr = runData.begin();
        std::vector<uint8_t>::iterator addrDataPtr = copyAddresses.begin();
        for(std::vector<uint8_t>::iterator instruction = instructionsSection.begin(); instruction < instructionsSection.end();){
            uint8_t cmd = *instruction; instruction++; // read value
        
            for(std::size_t i = 0; i < 2; i++){
                uint32_t sz = table[cmd][i].size;
                if(table[cmd][i].type != CodeType::NOP && table[cmd][i].size == 0){
                    sz = decodeInteger(instruction);
                }

                // Run the commands

                switch (table[cmd][i].type){
                case CodeType::ADD:
                    for(std::size_t chr = 0; chr < sz; chr++){
                        out.push_back(*addRunPtr);
                        addRunPtr++;
                    }
                    break;

                case CodeType::RUN:
                    for(std::size_t chr = 0; chr < sz; chr++){
                        out.push_back(*addRunPtr);
                    }
                    addRunPtr++;
                    break;

                case CodeType::COPY:
                    uint32_t addr = decodeInteger(addrDataPtr);
                    ka.DecodeAddress(addr,  ,table[cmd][i].mode);
                    break;

                case CodeType::NOP:
                    break;
                }
            }
        }

        out.shrink_to_fit(); // make sure we dont have a bunch of memory sitting around when we hand this back to the user

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
    if(static_cast<uint8_t>(flags) & static_cast<uint8_t>(SigmaFlags::Compressed)){
        indicator |= 1;
    }

    if(static_cast<uint8_t>(flags) & static_cast<uint8_t>(SigmaFlags::CodeTable)){
        indicator |= 2;
    }
    
    *writePtr++ = indicator;
    
    if(static_cast<uint8_t>(flags) & static_cast<uint8_t>(SigmaFlags::CodeTable)){
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