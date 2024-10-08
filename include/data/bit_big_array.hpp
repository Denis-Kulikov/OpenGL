#pragma once
#include "../lib-project/lib.hpp"
#include <vector>
#include <cstddef>
#include <cstdbool>
#include <stdexcept>

#include "bit_array.hpp"

class BitBigArray {
public:
    BitBigArray(ull_I totalSize, std::size_t numParts);
    ~BitBigArray();
    void set(ull_I index, bool value);
    bool get(ull_I index) const;
    uint8_t getByte(ull_I index) const;
    void zero();
    ull_I size() const;

// private:
    ull_I totalSize;    // Общий размер
    std::size_t numParts;     // Количество частей
    std::size_t partSize;     // Размер каждой части
    std::vector<BitArray> parts;          // Массив частей BitArray
};
