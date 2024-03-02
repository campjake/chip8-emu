#include <cstdint>
#include "../hpp/Chip8.hpp"

// Macro directive used to extract values from opcode in decode() fcn
#define SECOND_NIBBLE       0xF00   // X
#define THIRD_NIBBLE        0xF0    // Y
#define FOURTH_NIBBLE       0xF     // N   
#define SECOND_BYTE         0xFF00  // NN
#define IMMEDIATE_ADDRESS   0xFFF   // NNN (2nd, 3rd, & 4th nibbles)

struct Chip8::Impl {
    uint8_t registers[16]{};                        // Registers V0 - VF contain values ranging from 0x00 to 0xFF
	uint8_t memory[4096]{};                         // 4096 bytes of memory, from 0x000 to 0xFFF
	uint16_t index;                                 // Stores memory addresses for use in operations.
	uint16_t pc;                               // Contains the address of the next instruction to execute.
	uint16_t stack[16]{};                           // Stack can hold 16 different PCs for function call nesting.
	uint8_t sp;                                     // Stack Pointer to track where in memory the CPU is executing.
	uint8_t delayTimer;                             // Decrements at rate of cycle clock
	uint8_t soundTimer;                             // Sounds when timer is non-zero
	uint8_t keypad[16]{};                           // 16 hex values (0 through F)
	uint32_t video[Chip8::VIDEO_BUFFER_SIZE]{};    // Mem buffer for storing the graphics to display
	uint16_t opcode;                                // Instructions to execute
    uint8_t fontset[Chip8::FONTSET_SIZE] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

};

/** 
 * Default Constructor 
 * Preconditions    : N/A
 * Poscconditions   :   - pImpl and valid Impl,
 *                      - RNG bounds and vals set
 *                      - Program Counter points to valid START_ADDRESS
 *                      - Loads fonts into memory
*/
Chip8::Chip8()
:   pImpl(std::make_unique<Impl>()),
    randGen(std::chrono::system_clock::now().time_since_epoch().count()),
    randByte(std::uniform_int_distribution<uint8_t>(0, 255U))
{
    pImpl->pc = Chip8::START_ADDRESS;

    // Load fonts from Impl::fontset into main memory
    for(unsigned int i = 0; i < Chip8::FONTSET_SIZE; ++i) {
        pImpl->memory[Chip8::FONTESET_START_ADDRESS + i] = pImpl->fontset[i];
    }

}

/** 
 * Default Destructor 
 * Preconditions    : N/A
 * Postconditions   : N/A
 * */
Chip8::~Chip8() = default;

/** Move Constructor*/
Chip8::Chip8(Chip8&& rhs)
: pImpl(nullptr)
{ if (rhs.pImpl) pImpl = std::make_unique<Impl>(*rhs.pImpl); }

/** Move Assignment Operator */
Chip8& Chip8::operator=(Chip8&& rhs)
{
    if(!rhs.pImpl) {
        pImpl.reset();
    } else if (!pImpl) {
        pImpl = std::make_unique<Impl>(*rhs.pImpl);
    } else *pImpl = *rhs.pImpl;

    return *this;
}

/** LoadROM
 * Preconditions:
 *  - filename : Name of file containing the instructions
 * Postconditions: If sz is -1, alloc fails
 *  - 
*/
void Chip8::LoadROM(const char* filename) {
    // Open the file as a stream of binary and move the file ptr to EOF
    std::ifstream file(filename, std::ios::binary | std::ios::ate);

    if(file.is_open()) {
        // Get file size and alloc a buffer to hold contents 
        // Comes from the position of the current character in the istream
        std::streampos sz = file.tellg();
        std::unique_ptr<char> buf = std::make_unique<std::array<char, sz>>();

        file.seekg(0, std::ios::beg);   // Return to BOF
        file.read(buf, sz);             // Fill the buffer
        file.close();

        // Load ROM into Main Mem from 0x200
        for(long i = 0; i < sz; ++i) {
            pImpl->memory[Chip8::START_ADDRESS + i] = buf[i];
        }
    }
}

/**
 * Reads the instruction that the PC is pointing at from memory
 * Preconditions: Chip8::pc must contain a 2-byte instruction
 * Postconditions: Stores the instruction in Impl.opcode, and increments pc by 2
*/
void Chip8::fetch() {
    pImpl->opcode = pImpl->memory[pc++];    // 1 of 2 8-bit instructions
    pImpl->opcode = (instruction << 8u |    // 2 of 2 8-bit instructions
                    pImpl->memory[pc++]);   
}

/**
 * Function: decode
 * Preconditions    : Valid 16-bit instruction
 * Postconditions   : Calls the instruction's corresponding op-code fcn
*/
void Chip8::decode(unsigned int instruction) {
    #FIXME - needs a function pointer table!
}

/** 00E0: CLS
 * Clears the display
 * Preconditions    - N/A
 * Postconditions   - Video buffer contains only 0s
*/
void Chip8::OP_00E0() {
    memset(pImpl->video, 0, Chip8::VIDEO_BUFFER_SIZE);
}

/** 00EE: RET
 * Return from a subroutine
 * Preconditions    - N/A
 * Postconditions   - Changes the stack pointer & program counter
*/
void Chip8::OP_00EE() {
    --sp;
    pc = stack[sp];
}

/**
 * 1nnn: Jump
 * Preconditions    - N/A
 * Postconditions   - pc = NNN
*/
void Chip8::OP_1nnn() {

}

/**
 * 6xnn: Set register VX to NN
 * Preconditions    - N/A
 * Postconditions   - registers[VX] is set to NN
*/
void Chip8::OP_6xnn() {
    uint8_t vX = (pImpl->opcode & )
}