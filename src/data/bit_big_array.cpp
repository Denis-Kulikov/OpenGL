#include <data/bit_big_array.hpp>

BitBigArray::BitBigArray(ull_I totalSize, std::size_t numParts)
    : totalSize(totalSize), numParts(numParts), partSize(totalSize / numParts)
{
    std::cout << partSize << std::endl;
    for (std::size_t i = 0; i < numParts; ++i)
        parts.emplace_back(BitArray(partSize));
}

BitBigArray::~BitBigArray() {}

void BitBigArray::set(ull_I index, bool value) {
    std::size_t partIndex = index / partSize;
    std::size_t localIndex = index % partSize;
    parts[partIndex].set(localIndex, value);
}

bool BitBigArray::get(ull_I index) const {
    std::size_t partIndex = index / partSize;
    if (partIndex >= parts.size()) {
        std::cout << "index " << index << " partIndex " << partIndex << std::endl;
        throw std::out_of_range("Part index out of bounds");
    }
    std::size_t localIndex = index % partSize;
    return parts[partIndex].get(localIndex);
}

uint8_t BitBigArray::getByte(ull_I index) const {
    std::size_t partIndex = index / partSize;
    std::size_t localIndex = index % partSize;
    return parts[partIndex].getByte(localIndex);
}


void BitBigArray::zero() {
    for (auto &it : parts)
        it.zero();
}

ull_I BitBigArray::size() const {
    return totalSize;
}
