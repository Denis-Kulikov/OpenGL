#pragma once
#include "../lib-project/lib.hpp"
#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

class BitBigArray {
public:
    BitBigArray(ull_I partSize, std::size_t numParts);
    ~BitBigArray();
    void setBit(ull_I index, bool value);
    bool getBit(ull_I index) const;
    ull_I size() const;
    
// private:
    ull_I totalSize;    // Общий размер
    std::size_t numParts;     // Количество частей
    std::size_t partSize;     // Размер каждой части
    bool** parts;             // Массив указателей на подмассивы
};
