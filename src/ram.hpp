#ifndef CHIP_8_RAM

#define CHIP_8_RAM

#include <stddef.h>
#include <array>
#include <unordered_map>

#include "sprites.hpp"
#include "registers.hpp"
#include "exceptions.hpp"

namespace InterpretersAllocatedRam {
    void initalise_interpreter_memory(void);

    void init_memory_with_stored_sprites(const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>>&);
}

#endif