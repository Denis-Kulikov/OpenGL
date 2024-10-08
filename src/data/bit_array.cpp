
#include <data/bit_array.hpp>

BitArray::BitArray(std::size_t sizeInBits)
    : vec((sizeInBits + 7) / 8) {} 

BitArray::BitArray() {} 

void BitArray::set(std::size_t index, bool value) {
    // if (index >= (vec.size() * 8)) {
    //     throw std::out_of_range("Index out of bounds");
    // }
    std::size_t byteIndex = index / 8;
    std::size_t bitIndex = index % 8;

    if (value) 
        vec[byteIndex] |= (1 << bitIndex);
    else 
        vec[byteIndex] &= ~(1 << bitIndex);
}

bool BitArray::get(std::size_t index) const {
    if (index >= (vec.size() * 8)) {
        throw std::out_of_range("Index out of bounds");
    }
    std::size_t byteIndex = index / 8;
    std::size_t bitIndex = index % 8;
    return vec[byteIndex] & (1 << bitIndex);
}

uint8_t BitArray::getByte(std::size_t index) const {
    // if (index >= (vec.size() * 8)) {
    //     throw std::out_of_range("Index out of bounds");
    // }
    std::size_t byteIndex = index / 8;
    return vec[byteIndex];
}

bool BitArray::getBit(uint8_t data, std::size_t bitIndex) {
    return data & (1 << bitIndex);
}

void BitArray::zero() {
    for (auto it : vec)
        it = 0;
}

int BitArray::getSize() const {
    return vec.size() * 8;
}

