#include "timer.h"
#include <random>  // for mersenne twister
#include <array>
#include <cstdint>
#include <iostream>

constexpr int g_arraySize{4000};

int main() {
    std::mt19937_64 rng(0);  // TODO: segfaults here for some reason SPECIFICALLY for debug

    // Test 1: Row Major Traversal
    Timer timer{};
    std::array<std::array<int, g_arraySize>, g_arraySize> rowMajorMatrix{};
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            rowMajorMatrix[i][j] = rng();
        }
    }
    std::cout << "Row Major Traversal: " << timer.glance<Timer::Micros>() << '\n';

    // Test 2: Col Major Traversal
    rng.seed(0);
    std::array<std::array<int, g_arraySize>, g_arraySize> colMajorMatrix{};
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            colMajorMatrix[j][i] = rng();
        }
    }
    std::cout << "Col Major Traversal: " << timer.glance<Timer::Micros>() << '\n';

}


