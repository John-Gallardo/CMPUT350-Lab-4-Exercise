#include "p2a.h"
#include "timer.h"
#include <cstdint>
#include <iostream>
#include <random>
#include <cassert>

constexpr int g_arraySize{256};
constexpr int g_kernelSize{4};
constexpr int g_stride{4};
constexpr int g_convolutionSize{(g_arraySize - g_kernelSize) / 4 + 1}; 

size_t rowMajorIndexA(size_t x, size_t y, size_t z) {
    // this should be the correct formula?
    return (x * g_arraySize * g_arraySize) + (y * g_arraySize) + z;
}

size_t rowMajorIndexK(size_t x, size_t y, size_t z) {
    return (x * g_kernelSize * g_kernelSize) + (y * g_kernelSize) + z;
}

size_t rowMajorIndexConv(size_t x, size_t y, size_t z) {
    return (x * g_convolutionSize * g_convolutionSize) + (y * g_convolutionSize) + z;
}

int main() {
    std::mt19937_64 rng(0);

    // A: row-major order
    std::vector<uint64_t> rowMajorArray(g_arraySize * g_arraySize * g_arraySize, 0);
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            for (int k{0}; k < g_arraySize; k++) {
                rowMajorArray[rowMajorIndexA(i, j, k)] = rng();
            }
        }
    }
    
    // B: Morton-order array
    std::vector<uint64_t> mortonArray(g_arraySize * g_arraySize * g_arraySize, 0);
    for (int i{0}; i < g_arraySize; i++) {
        for (int j{0}; j < g_arraySize; j++) {
            for (int k{0}; k < g_arraySize; k++) {
                mortonArray[morton3d(i, j, k)] = rowMajorArray[rowMajorIndexA(i, j, k)];
            }
        }
    }

    // Convolution
    // initialize row major kernel & morton kernel
    std::vector<uint64_t> rowMajorKernel(g_kernelSize * g_kernelSize * g_kernelSize, 0);
    for (int i{0}; i < g_kernelSize; i++) {
        for (int j{0}; j < g_kernelSize; j++) {
            for (int k{0}; k < g_kernelSize; k++) {
                rowMajorKernel[rowMajorIndexK(i, j, k)] = i + j + k;
            }
        }
    }

    std::vector<uint64_t> mortonKernel(g_kernelSize * g_kernelSize * g_kernelSize, 0);
    for (int i{0}; i < g_kernelSize; i++) {
        for (int j{0}; j < g_kernelSize; j++) {
            for (int k{0}; k < g_kernelSize; k++) {
                mortonKernel[morton3d(i, j, k)] = i + j + k;
            }
        }
    }

    // time convolutions
    Timer timer{};

    // formula from lab description
    std::vector<uint64_t> convolutionA(g_convolutionSize * g_convolutionSize * g_convolutionSize, 0);
    timer.restart();
    // outer 3 loops -> iterate through every convolution slot
    for (int i{0}; i < g_convolutionSize; i++) {
        for (int j{0}; j < g_convolutionSize; j++) {
            for (int k{0}; k < g_convolutionSize; k++) {
                // inner 3 loops -> compute our convolution dot product
                uint64_t convolutionValue{0};
                for (int a{0}; a < g_kernelSize; a++) {
                    for (int b{0}; b < g_kernelSize; b++) {
                        for (int c{0}; c < g_kernelSize; c++) {
                            // NOTE: idk if this is correct
                            convolutionValue += rowMajorArray[rowMajorIndexA(i + a, j + b, k + c)] * rowMajorKernel[(a * g_kernelSize * g_kernelSize) + (b * g_kernelSize) + c];
                        }
                    }
                }
                convolutionA[rowMajorIndexConv(i, j, k)] = convolutionValue;
            }
        }
    }
    std::cout << timer.glance<Timer::Micros>() << '\n';

    /*
    for (int i{0}; i < g_kernelSize; i++) {
        for (int j{0}; j < g_kernelSize; j++) {
            for (int k{0}; j < g_kernelSize; k++) {
                size_t convolutionIndex{rowMajorIndexConv(i, j, k)};
                // inner 3 for loops -> for convolution
                for (int a{0}; a < g_kernelSize; a++) {
                    for (int b{0}; b < g_kernelSize; b++) {
                        for (int c{0}; c < g_kernelSize; c++) {
                            convolutionA[convolutionIndex] += rowMajorArray[rowMajorIndexA(i, j, k) + rowMajorIndexA(a, b, c)] * rowMajorKernel[convolutionIndex];
                        }
                    }
                }
            }
        }
    }
    std::cout << timer.glance<Timer::Micros>() << '\n';

    std::vector<uint64_t> convolutionB(convolutionSize * convolutionSize * convolutionSize, 0); 
    timer.restart();
    for (int i{0}; i < g_kernelSize; i++) {
        for (int j{0}; j < g_kernelSize; j++) {
            for (int k{0}; k < g_kernelSize; k++) {
                size_t convolutionIndex{rowMajorIndexConv(i, j, k)};
                // inner 3 for loops -> for convolution
                for (int a{0}; a < g_kernelSize; a++) {
                    for (int b{0}; b < g_kernelSize; j++) {
                        for (int c{0}; c < g_kernelSize; c++) {
                            convolutionB[convolutionIndex] += mortonArray[morton3d(i, j, k) + morton3d(a, b, c)] * mortonKernel[convolutionIndex];
                        }
                    }
                }
            }
        }
    }
    std::cout << timer.glance<Timer::Micros>() << '\n';

    // assert convolutions are equal
    for (int i{0}; i < g_kernelSize; i++) {
        for (int j{0}; j < g_kernelSize; j++) {
            for (int k{0}; k < g_kernelSize; k++) {
                size_t convolutionIndex{rowMajorIndexConv(i, j, k)};
                assert(convolutionA[convolutionIndex] == convolutionB[convolutionIndex]);
            }
        }
    }
    */

    return 0;
}
