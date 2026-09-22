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
    std::vector<uint64_t> rowMajorMatrix(g_arraySize * g_arraySize, 0);
    uint64_t sum{};
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            uint64_t num{rng()};
            rowMajorMatrix[(i * g_arraySize) + j] = num;
            sum += num;
        }
    }
    std::cout << timer.glance<Timer::Micros>() << ' ' << sum << '\n';

    // Test 2: Col Major Traversal
    rng.seed(0);
    std::vector<uint64_t> colMajorMatrix(g_arraySize * g_arraySize, 0);
    sum = 0;
    timer.restart();
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            uint64_t num{rng()};
            colMajorMatrix[(j * g_arraySize) + i] = num;
            sum += num;
        }
    }
    std::cout << timer.glance<Timer::Micros>() << ' ' << sum << '\n';

}


