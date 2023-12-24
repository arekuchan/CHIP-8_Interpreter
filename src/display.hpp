#ifndef CHIP_8_SPRITES_H

#define CHIP_8_SPRITES_H

#include <cstdint>
#include <array>
#include <map>

namespace StoredSprites {
    inline constexpr int numRowsOccupied = 5; // of each stored 

    extern const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>> storedSpriteMap;
}

#endif