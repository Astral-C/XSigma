#pragma once
#include <cstdint>
#include <cstddef>
#include <vector>
#include <cstring>
// implements VCDiff RFC 3284 https://www.rfc-editor.org/rfc/rfc3284.txt
// should be compatible with xdelta3

namespace XSigma {

enum SigmaCompressionType {
    None,
    Gzip
};

enum SigmaFlags {
    Compressed, // secondary compression
    CodeTable  // secondary code table
};

// vcdiff decode
void Decode(std::vector<uint8_t>& source, std::vector<uint8_t>& target, std::vector<uint8_t>& out);
void DecodeBuffer(uint8_t* sourceBuffer, std::size_t soruceBufferSize, uint8_t* targetBuffer, std::size_t targetBufferSize, uint8_t* outputBuffer, std::size_t& outSize);


// vcdif encode
void Encode(std::vector<uint8_t>& source, std::vector<uint8_t>& target, std::vector<uint8_t>& delta);
void EncodeBuffer(uint8_t* sourceBuffer, std::size_t soruceBufferSize, uint8_t* targetBuffer, std::size_t targetBufferSize, uint8_t* deltaBuffer, std::size_t& deltaSize);

}