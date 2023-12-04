#include "ram.hpp"

namespace InterpretersAllocatedRam {
    // this is in bytes
    constexpr int amtInterpreterMem = 512;

    std::array<std::byte, amtInterpreterMem> interpreterMem{};
    
    // gets initialised by init_memory_with_stored_sprites
    // contains the offset addrs in the allocated interpreter memory
    // of where the sprite bit patterns start
    std::unordered_map<char, int> storedSpritesLocations{};

    void initalise_interpreter_memory(void) {
        init_memory_with_stored_sprites(StoredSprites::storedSpriteMap);
    }

    void init_memory_with_stored_sprites(const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>>& storedSpritesMap) {
        int memOffset = 0;

        for (const auto& [key, value] : storedSpritesMap) {
            for (int i = 0; i < StoredSprites::numRowsOccupied; i++) {
                interpreterMem[memOffset + i] = value[i];
            }

            storedSpritesLocations.insert({key, memOffset});
            memOffset += StoredSprites::numRowsOccupied;
        }
    }
}

namespace RamOperations {

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

    // make sure storedSpritesLocations is initialised properly before calling this
    void set_i_to_stored_sprite_loc_FX29(std::int8_t vXAddr) {
        auto&& vX = Registers::variableRegistersMap.at(vXAddr);
        char spriteChar = hexDigitToChar(vX);

        if (spriteChar == '\0') {
            throw InvalidStoredSpriteIdentifierException(vX);
        }

        int memOffset = InterpretersAllocatedRam::storedSpritesLocations.at(spriteChar);

        if (Registers::i_register_wont_overflow(memOffset)) {
            Registers::set_i_register(memOffset);
        } else {
            std::stringstream errMsg;
            errMsg << "Register-I size: " << Registers::iRegisterSize << "bits can't store mem address: " << memOffset;

            throw std::overflow_error(errMsg.str());
        }
    }
}