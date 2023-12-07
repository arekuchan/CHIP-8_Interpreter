#include "ram.hpp"

namespace Chip8VMRam {
    // this is in bytes
    constexpr int amtRam = 4096;

    constexpr int storedSpritesStartingOffset = 0;

    std::array<std::byte, amtRam> chip8VMRam{};
    
    // gets initialised by init_memory_with_stored_sprites
    // contains the offset addrs in the allocated interpreter memory
    // of where the sprite bit patterns start
    std::unordered_map<char, int> storedSpritesLocations{};

    void initalise_interpreter_memory(void) {
        init_memory_with_stored_sprites(StoredSprites::storedSpriteMap);
    }

    void init_memory_with_stored_sprites(const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>>& storedSpritesMap) {
        int memOffset = storedSpritesStartingOffset;

        for (const auto& [key, value] : storedSpritesMap) {
            for (int i = 0; i < StoredSprites::numRowsOccupied; i++) {
                chip8VMRam[memOffset + i] = value[i];
            }

            storedSpritesLocations.insert({key, memOffset});
            memOffset += StoredSprites::numRowsOccupied;
        }
    }

    void write_byte_to_ram(std::byte data, auto addr) {
        chip8VMRam[addr] = data;
    }

    std::byte read_byte_from_ram(auto addr) {
        return chip8VMRam[addr];
    }
}

namespace RamOpcodes {
    // Will return the char representation of a hex digit
    // i.e 0x0F -> 'F'
    // if invalid hex pattern provided, the null char is returned
    char hexDigitToChar(std::int8_t hexDigit) {
        if (0x00 <= hexDigit && hexDigit <= 0x09) {
            return '0' + hexDigit;
        } else if (0x0A <= hexDigit && hexDigit <= 0x0F) {
            return 'A' + (hexDigit - 0x0A);
        }

        return '\0';
    }

    // TODO : change var names of vXAddr to vXID
    // As they aren't addresses

    // make sure storedSpritesLocations is initialised properly before calling this
    void set_i_to_stored_sprite_loc_FX29(std::int8_t vXAddr) {
        auto&& vX = Registers::variableRegistersMap.at(vXAddr);
        char spriteChar = hexDigitToChar(vX);

        if (spriteChar == '\0') {
            throw InvalidStoredSpriteIdentifierException(vX);
        }

        int memOffset = Chip8VMRam::storedSpritesLocations.at(spriteChar);

        if (Registers::i_register_wont_overflow(memOffset)) {
            Registers::set_i_register(memOffset);
        } else {
            std::stringstream errMsg;
            errMsg << "Register-I size: " << Registers::iRegisterSize << "bits can't store mem address: " << memOffset;

            throw std::overflow_error(errMsg.str());
        }
    }

    // throws OutOfChip8VmRamException if there isn't
    void check_enough_ram_to_access(auto startAddr, auto endAddr) {
        if (startAddr > Chip8VMRam::amtRam || endAddr > Chip8VMRam::amtRam) {
            throw OutOfChip8VmRamException();
        }
    }

    // vx inclusive
    void str_from_v0_to_vx_into_mem_FX55(std::int8_t vXId) {
        auto currAddr = Registers::get_i_register();

        check_enough_ram_to_access(currAddr, currAddr + vXId);

        for (int i : std::views::iota(0, vXId + 1)) {
            int8_t currData = Registers::variableRegistersMap.at(i);
            Chip8VMRam::write_byte_to_ram(static_cast<std::byte>(currData), currAddr);

            currAddr += 1;
        }
    }

    // vx inclusive
    void ld_from_memory_into_v0_to_vx_FX65(std::int8_t vXId) {
        auto currAddr = Registers::get_i_register();

        check_enough_ram_to_access(currAddr, currAddr + vXId);

        for (int i : std::views::iota(0, vXId + 1)) {
            std::byte currData = Chip8VMRam::read_byte_from_ram(currAddr);
            auto& vXRegister = Registers::variableRegistersMap.at(i);

            vXRegister = static_cast<std::int8_t>(currData);
            currAddr += 1;
        }
    }
}