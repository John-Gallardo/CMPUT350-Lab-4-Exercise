#include "p2a.h"
#include "timer.h"
#include <cstdint>
#include <iostream>
#include <random>

constexpr int g_arraySize{256};

int main() {
    Timer timer{};
    std::mt19937_64 rng(0);

    // A: row-major order
    std::vector<uint64_t> rowMajorArray(g_arraySize * g_arraySize * g_arraySize, 0);
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            for (int k{0}; k < g_arraySize; k++) {
                // this should be the correct formula?
                rowMajorArray[(i * g_arraySize * g_arraySize) + (j * g_arraySize) + k] = rng();
            }
        }
    }
    std::cout << timer.glance<Timer::Micros>() << '\n';
    
    // B: Morton-order array

    return 0;
}
