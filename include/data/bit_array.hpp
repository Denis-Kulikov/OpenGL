#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

class BitArray {
public:
    BitArray(std::size_t sizeInBits);
    BitArray();
    void setBit(std::size_t index, bool value);
    bool getBit(std::size_t index) const;
    int getSize() const;

private:
    std::vector<uint8_t> vec;
};

