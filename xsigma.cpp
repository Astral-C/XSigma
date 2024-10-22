#include <xsigma.hpp>

namespace XSigma {

// vcdiff decode
void Decode(std::vector<uint8_t>& target, std::vector<uint8_t>& patch, std::vector<uint8_t>& out){
    // Get some information about patches
    std::vector<uint8_t>::iterator patchReadPtr = patch.begin();

    uint32_t magic;
    uint8_t indicator;
    uint8_t compressionType;

    uint8_t nearCacheSize, sameCacheSize;
    

    std::memcpy(&magic, &(*patchReadPtr), sizeof(uint32_t));
    indicator = *patchReadPtr; patchReadPtr++;

    if(indicator & SigmaFlags::Compressed){
        compressionType = *patchReadPtr; patchReadPtr; patchReadPtr++;
    }

    if(indicator & SigmaFlags::CodeTable){
        nearCacheSize = *patchReadPtr; patchReadPtr++;
        sameCacheSize = *patchReadPtr; patchReadPtr++;    
    
        patchReadPtr += nearCacheSize + sameCacheSize; // skip internal code table for now
    }

    bool hasRemainingWindows = true;

    while(hasRemainingWindows){
        uint8_t curWindowIndicator = *patchReadPtr; patchReadPtr++;
        uint32_t segmentSize, segmentPosition;

        std::memcpy(&segmentSize, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);
        std::memcpy(&segmentPosition, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);


        bool isSourceWindow = curWindowIndicator & 0x01; // pull from source file
        bool isTargetWindow = curWindowIndicator & 0x02; // pull from target data

        if(isSourceWindow && isTargetWindow){
            // err only one of these is valid!!!
            return;
        }

        // Handle delta encoding data
        uint32_t deltaEncodingSize, targetWindowSize;

        std::memcpy(&deltaEncodingSize, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);
        std::memcpy(&targetWindowSize, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);

        uint8_t deltaIndicator = *patchReadPtr; patchReadPtr++;

        uint32_t addRunDataSize, instructionsSectionSize, copyAddrsSize;

        std::memcpy(&addRunDataSize, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);
        std::memcpy(&instructionsSectionSize, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);
        std::memcpy(&copyAddrsSize, &(*patchReadPtr), sizeof(uint32_t)); patchReadPtr += sizeof(uint32_t);
    

    
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