#include "p2a.h"
#include "timer.h"
#include <cstdint>
#include <iostream>
#include <random>

constexpr int g_arraySize{256};

int rowMajorIndexA(int x, int y, int z) {
    // this should be the correct formula?
    return (x * g_arraySize * g_arraySize) + (y * g_arraySize) + z;
}

int main() {
    Timer timer{};
    std::mt19937_64 rng(0);

    // A: row-major order
    std::vector<uint64_t> rowMajorArray(g_arraySize * g_arraySize * g_arraySize, 0);
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            for (int k{0}; k < g_arraySize; k++) {
                rowMajorArray[rowMajorIndexA(i, j, k)] = rng();
            }
        }
    }
    std::cout << timer.glance<Timer::Micros>() << '\n';
    
    // B: Morton-order array
    std::vector<uint64_t> mortonArray(g_arraySize * g_arraySize * g_arraySize, 0);
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            for (int k{0}; k < g_arraySize; k++) {
                mortonArray[morton3d(i, j, k)] = rowMajorArray[rowMajorIndexA(i, j, k)];
            }
        }
    }
    std::cout << timer.glance<Timer::Micros>() << '\n';
    return 0;
}
