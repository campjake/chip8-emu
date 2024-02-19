#ifndef CHIP_8_H_
#define CHIP_8_H_

#include <memory>
#include <chrono>
#include <random>
#include <fstream>
#include <array>

class Chip8 {
public:
    Chip8();
    ~Chip8();

    Chip8(Chip8&& rhs);
    Chip8& operator=(Chip8&& rhs);

    void LoadROM(const char* filename);    // Loads instructions into memory

   void OP_00E0();  // CLS
   void OP_00EE();  // RET
   void OP_1nnn();  // JP addr
   void OP_2nnn();  // CALL addr
   void OP_3xkk(const unsigned int Vx);  // SE Vx, byte

// Member Variables
    const static unsigned int START_ADDRESS = 0x200;
    const static unsigned int FONTSET_SIZE  = 80;
    const static unsigned int FONTESET_START_ADDRESS = 0x50;
    constexpr unsigned int VIDEO_BUFFER_SIZE = 64 * 32;


private:
    struct Impl;                    // Implementation of memory
    std::unique_ptr<Impl> pImpl;    // ptr to Implementation
    std::default_random_engine randGen;                 // RNG
    std::uniform_int_distribution<uint8_t> randByte;    // RNG range [0, 255]

};

#endif // CHIP_8_H_