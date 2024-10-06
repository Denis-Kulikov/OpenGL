
#include <data/bit_big_array.hpp>


BitBigArray::BitBigArray(ull_I totalSize, std::size_t numParts)
    : totalSize(totalSize), numParts(numParts), partSize(totalSize / numParts)
{
    parts = new bool*[numParts];
    for (std::size_t i = 0; i < numParts; ++i) {
        parts[i] = new bool[partSize];
    }
}

BitBigArray::~BitBigArray() {
    for (std::size_t i = 0; i < numParts; ++i) {
        delete[] parts[i];
    }
    delete[] parts;
}

void BitBigArray::setBit(ull_I index, bool value) {
    std::size_t partIndex = index / partSize;
    std::size_t localIndex = index % partSize;
    parts[partIndex][localIndex] = value;
}

bool BitBigArray::getBit(ull_I index) const {
    std::size_t partIndex = index / partSize;
    std::size_t localIndex = index % partSize;
    return parts[partIndex][localIndex];
}

ull_I BitBigArray::size() const {
    return totalSize;
}
