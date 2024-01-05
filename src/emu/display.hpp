#ifndef CHIP_8_SPRITES_H

#define CHIP_8_SPRITES_H

#include <cstdint>
#include <array>
#include <map>

#include <string>
#include <string_view>
#include <iostream>

#include <Eigen/Dense>
#include <yaml-cpp/yaml.h>
#include <SDL2/SDL.h>

#include "exceptions.hpp"

namespace StoredSprites {
    inline constexpr int numRowsOccupied = 5; // of each stored 

    extern const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>> storedSpriteMap;
}

namespace DisplayOpcodes {
    void disp_clear_00E0(void);

    void disp_draw_DXYN(std::int8_t, std::int8_t, int);
}

namespace Config {
    const std::string configFilePath = "../config/config.yaml";

    const std::string resToken = "Chip-8-Display-Resolution";

    const std::string resWidthToken = "Width";
    const std::string resHeightToken = "Height";

    int get_res_field(const std::string_view);

    int get_res_height(void);

    int get_res_width(void);
}

namespace RenderEngine {
    constexpr int internalStateWidth = 64;
    constexpr int internalStateHeight = 32;

    void clear_internal_display_state(void);

    void init_interal_game_state(void);

    void display_current_state_on_screen(int, int);

    void init_render_engine(void);

    void reset_display();
}

#endif