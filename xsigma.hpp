#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstring>
#include <array>

// implements VCDiff RFC 3284 https://www.rfc-editor.org/rfc/rfc3284.txt
// should be compatible with xdelta3
namespace XSigma {

enum class CodeType {
    NOP,
    ADD,
    RUN,
    COPY
};

struct Code {
    CodeType type;
    uint8_t size;
    uint8_t mode;
};

typedef std::array<Code[2], 256> CodeTable;

extern CodeTable DefaultCodeTable;

struct Cache {
    uint32_t nextSlot { 0 }; // rings around near 
    std::vector<uint32_t> near {};
    std::vector<uint32_t> same {};

    Cache();
    Cache(uint32_t s_near, uint32_t s_same);
    
    void Update(uint32_t address);
    uint32_t DecodeAddress(std::vector<uint8_t>::iterator buff, uint32_t here, uint32_t mode);
};

enum SigmaCompressionType {
    None,
    DJW  = 0x01,
    LZMA = 0x02,
    FGK  = 0x10
};

enum class SigmaFlags {
    Compressed = 0x01, // secondary compression
    CodeTable = 0x02, // secondary code table
    ApplicationData = 0x04 // has extra application data
};

// vcdiff decode
void Decode(std::vector<uint8_t>& target, std::vector<uint8_t>& patch, std::vector<uint8_t>& out);
void DecodeBuffer(uint8_t* sourceBuffer, std::size_t soruceBufferSize, uint8_t* targetBuffer, std::size_t targetBufferSize, uint8_t* outputBuffer, std::size_t& outSize);


// vcdif encode
void Encode(std::vector<uint8_t>& source, std::vector<uint8_t>& target, std::vector<uint8_t>& delta);
void EncodeBuffer(uint8_t* sourceBuffer, std::size_t soruceBufferSize, uint8_t* targetBuffer, std::size_t targetBufferSize, uint8_t* deltaBuffer, std::size_t& deltaSize);

}