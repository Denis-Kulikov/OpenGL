#pragma once
#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

class BitBigArray {
public:
    BitBigArray(unsigned long long partSize, std::size_t numParts);
    ~BitBigArray();
    void setBit(std::size_t index, bool value);
    bool getBit(std::size_t index) const;
    std::size_t size() const;
    
private:
    unsigned long long totalSize;    // Общий размер
    std::size_t numParts;     // Количество частей
    std::size_t partSize;     // Размер каждой части
    bool** parts;             // Массив указателей на подмассивы
};
