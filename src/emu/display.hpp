#ifndef CHIP_8_SPRITES_H

#define CHIP_8_SPRITES_H

#include <cstdint>
#include <array>
#include <map>

#include <string>
#include <string_view>
#include <iostream>
#include <ranges>

#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>
#include <memory>

#include "exceptions.hpp"
#include "misc.hpp"
#include "config.hpp"

namespace Sprites {
    inline constexpr int spriteWidth = 8;

    using SpriteMatrix = Eigen::Matrix<int, Eigen::Dynamic, spriteWidth>;
}

namespace StoredSprites {
    inline constexpr int numRowsOccupied = 5; // of each stored 

    extern const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>> storedSpriteMap;
}

// this include is here on purpose, the program won't compile if it isn't in this order (lol)
// *don't do cyclic dependicies kids*
#include "registers.hpp"

namespace DisplayOpcodes {
    bool disp_clear_00E0(void);

    void disp_draw_DXYN(std::int8_t, std::int8_t, int);
}

namespace RenderEngine {
    constexpr int internalStateWidth = 64;
    constexpr int internalStateHeight = 32;

    void display_current_state_on_screen(int, int);

    void xor_sprite_on_screen(const Sprites::SpriteMatrix&, int, int);

    void init_render_engine(void);

    void reset_display();

    int adjust_res_dimension(int, bool);

    int adjust_res_height(int);

    int adjust_res_width(int);
}

#endif
