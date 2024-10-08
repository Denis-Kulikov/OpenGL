#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

class BitArray {
public:
    BitArray(std::size_t sizeInBits);
    BitArray();
    void set(std::size_t index, bool value);
    bool get(std::size_t index) const;
    uint8_t getByte(std::size_t index) const;
    static bool getBit(uint8_t data, std::size_t bitIndex);
    void zero();
    int getSize() const;

private:
    std::vector<uint8_t> vec;
};

