#include "xsigma.hpp"
#include <fstream>

int main(int argc, char* argv[]){
    if(argc < 3){
        return 1;
    }

    std::vector<uint8_t> targetBuffer, patchBuffer, outBuffer;

    std::fstream targetFile(std::string(argv[1]), std::ios_base::openmode::_S_in);
    
    targetFile.seekg(0, std::ios_base::end);
    targetBuffer.resize(targetFile.tellg());
    targetFile.seekg(0, std::ios_base::beg);

    targetFile.read(reinterpret_cast<char*>(targetBuffer.data()), targetBuffer.size());

    std::fstream patchFile(std::string(argv[2]), std::ios_base::openmode::_S_in);
    
    patchFile.seekg(0, std::ios_base::end);
    patchBuffer.resize(patchFile.tellg());
    patchFile.seekg(0, std::ios_base::beg);

    patchFile.read(reinterpret_cast<char*>(patchBuffer.data()), patchBuffer.size());

    XSigma::Decode(targetBuffer, patchBuffer, outBuffer);
    
    return 0;
}