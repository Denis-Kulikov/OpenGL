
#include <data/bit_array.hpp>

BitArray::BitArray(std::size_t sizeInBits)
    : vec((sizeInBits + 7) / 8) {} 

BitArray::BitArray() {} 

void BitArray::setBit(std::size_t index, bool value) {
    if (index >= (vec.size() * 8)) {
        throw std::out_of_range("Index out of bounds");
    }
    std::size_t byteIndex = index / 8;
    std::size_t bitIndex = index % 8;

    if (value) 
        vec[byteIndex] |= (1 << bitIndex);
    else 
        vec[byteIndex] &= ~(1 << bitIndex);
}

bool BitArray::getBit(std::size_t index) const {
    if (index >= (vec.size() * 8)) {
        throw std::out_of_range("Index out of bounds");
    }
    std::size_t byteIndex = index / 8;
    std::size_t bitIndex = index % 8;
    return vec[byteIndex] & (1 << bitIndex);
}

int BitArray::getSize() const {
    return vec.size() * 8;
}

