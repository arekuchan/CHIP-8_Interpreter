#ifndef CHIP_8_SPRITES_H

#define CHIP_8_SPRITES_H

#include <cstdint>
#include <array>
#include <map>
#include <iostream>

#include <Eigen/Dense>

namespace StoredSprites {
    inline constexpr int numRowsOccupied = 5; // of each stored 

    extern const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>> storedSpriteMap;
}

namespace DisplayOpcodes {
    void disp_clear_00E0(void);

    void disp_draw_DXYN(std::int8_t, std::int8_t, int);
}

namespace Renderer {
    constexpr int internalStateWidth = 64;
    constexpr int internalStateHeight = 32;

    void clear_internal_display_state(void);

    void init_interal_game_state(void);

    void display_current_state_on_screen(int, int);
}

#endif