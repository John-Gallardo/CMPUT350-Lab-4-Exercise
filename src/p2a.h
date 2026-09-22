#pragma once  // header guard
#include <cstdint>

// from my lab prep sol
inline uint64_t expand(uint64_t input, uint32_t scale) {
    // NOTE: this should technically be O(wordsize) since the upper bound of numBits is 64
    // i.e we cost for 1. and 2. is O(wordsize) + O(wordsize) = O(2wordsize) = O(wordsize)

    // 1. we count number of bits in input. eg. 0b1111 -> 4 bits
    int numBits{};
    uint64_t inputCopy{input};
    while (inputCopy) {
        numBits++;
        inputCopy >>= 1;
    }

    // 2. set bits
    uint64_t expandedBits{0};
    int currBitToSet{};  // 0-63
    for (int i{0}; i < numBits && currBitToSet < 64; i++) {
        uint64_t currVal{input & 0x1};
        expandedBits |= currVal << currBitToSet;
        // scale - 1 by definition, + 1 since we are moving onto the next bit
        currBitToSet += (scale - 1) + 1;
        input >>= 1;  // move onto next value to set
    }
    return expandedBits;
}

inline uint64_t morton3d(uint64_t x, uint64_t y, uint64_t z) {
    // no need to bitshift x
    y <<= 1;
    z <<= 2;

    uint64_t expandedX{expand(x, 3)};  // scale == d == 3
    uint64_t expandedY{expand(y, 3)}; 
    uint64_t expandedZ{expand(z, 3)}; 
    return expandedX | expandedY | expandedZ;
}

