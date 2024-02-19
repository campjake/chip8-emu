#include <gtest/gtest.h>
#include <iostream>
#include <string.h>

#include "../src/hpp/Chip8.hpp"

TEST(Chip8Ctor, BasicAssertions) {
    // Testing ctor by passing size
    Chip8 myChip;
    uint8_t pcAddr = myChip.getPC();
    std::cout << "pcAddr: " << pcAddr << std::endl;
}