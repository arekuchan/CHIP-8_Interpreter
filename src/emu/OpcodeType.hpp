#ifndef CHIP_8_OPCODE_TYPE

#define CHIP_8_OPCODE_TYPE

#include <cstdint>
#include <array>
#include <cstddef>

// in bytes
constexpr size_t opcodeSize = 2;

const size_t opcodeArgBitSize = 4;

typedef std::array<std::byte, opcodeSize> Chip8Opcode;

#endif
