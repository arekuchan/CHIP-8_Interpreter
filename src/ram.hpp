#ifndef CHIP_8_RAM

#define CHIP_8_RAM

#include <stddef.h>
#include <array>
#include <unordered_map>

#include <ranges>

#include "sprites.hpp"
#include "registers.hpp"
#include "exceptions.hpp"

namespace Chip8VMRam {
    void initalise_interpreter_memory(void);

    void init_memory_with_stored_sprites(const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>>&);

    void write_byte_to_ram(std::byte, auto);

    std::byte read_byte_from_ram(auto);
}

namespace RamOpcodes {
    char hexDigitToChar(std::int8_t);

    void set_i_to_stored_sprite_loc_FX29(std::int8_t);

    void check_enough_ram_to_access(auto, auto);

    void str_from_v0_to_vx_into_mem_FX55(std::int8_t);

    void ld_from_memory_into_v0_to_vx_FX65(std::int8_t);

    bool set_addr_i_register_ANNN(std::int16_t);
}

#endif