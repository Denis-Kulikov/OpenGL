#pragma once
#include "../lib-project/lib.hpp"
#include <vector>
#include <cstddef>
#include <cstdint>
#include <stdexcept>

template<typename T>
class BigArray {
public:
    BigArray(ull_I totalSize, std::size_t numParts)
        : totalSize(totalSize), numParts(numParts), partSize(totalSize / numParts)
    {
        parts = new bool*[numParts];
        for (std::size_t i = 0; i < numParts; ++i) {
            parts[i] = new bool[partSize];
        }
    }

    ~BigArray() {
        for (std::size_t i = 0; i < numParts; ++i) {
            delete[] parts[i];
        }
        delete[] parts;
    }

    void setV(ull_I index, T value) {
        std::size_t partIndex = index / partSize;
        std::size_t localIndex = index % partSize;
        parts[partIndex][localIndex] = value;
    }

    T getV(ull_I index) const {
        std::size_t partIndex = index / partSize;
        std::size_t localIndex = index % partSize;
        return parts[partIndex][localIndex];
    }

    ull_I size() const {
        return totalSize;
    }

    
// private:
    ull_I totalSize;    // Общий размер
    std::size_t numParts;     // Количество частей
    std::size_t partSize;     // Размер каждой части
    T** parts;             // Массив указателей на подмассивы
};
