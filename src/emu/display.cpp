#include "display.hpp"

namespace StoredSprites {
    // map char to its sprite representation
    // representation are stored as rows of bytes
    // each element in the array is a byte that represents a row
    // and the order is from top row to bottom row
    //
    // the bytes represent setting pixels on or off

    // Here's the mappings translated to something readable:
    // '0' -> {0xF0, 0x90, 0x90, 0x90, 0xF0}
    // '1' -> {0x20, 0x60, 0x20, 0x20, 0x70}
    // '2' -> {0xF0, 0x10, 0xF0, 0x80, 0xF0}
    // '3' -> {0xF0, 0x10, 0xF0, 0x10, 0xF0}
    // '4' -> {0x90, 0x90, 0xF0, 0x10, 0x10}
    // '5' -> {0xF0, 0x80, 0xF0, 0x10, 0xF0}
    // '6' -> {0xF0, 0x80, 0xF0, 0x90, 0xF0}
    // '7' -> {0xF0, 0x10, 0x20, 0x40, 0x40}
    // '8' -> {0xF0, 0x90, 0xF0, 0x90, 0xF0}
    // '9' -> {0xF0, 0x90, 0xF0, 0x10, 0xF0}
    // 'A' -> {0xF0, 0x90, 0xF0, 0x90, 0x90}
    // 'B' -> {0xE0, 0x90, 0xE0, 0x90, 0xE0}
    // 'C' -> {0xF0, 0x80, 0x80, 0x80, 0xF0}
    // 'D' -> {0xE0, 0x90, 0x90, 0x90, 0xE0}
    // 'E' -> {0xF0, 0x80, 0xF0, 0x80, 0xF0}
    // 'F' -> {0xF0, 0x80, 0xF0, 0x80, 0x80}

    const std::map<char, std::array<std::byte, StoredSprites::numRowsOccupied>> storedSpriteMap{
         {'0', {{std::byte{0xF0}, std::byte{0x90}, std::byte{0x90}, std::byte{0x90}, std::byte{0xF0}}}},
         {'1', {{std::byte{0x20}, std::byte{0x60}, std::byte{0x20}, std::byte{0x20}, std::byte{0x70}}}},
         {'2', {{std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0}}}},
         {'3', {{std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0}}}},
         {'4', {{std::byte{0x90}, std::byte{0x90}, std::byte{0xF0}, std::byte{0x10}, std::byte{0x10}}}},
         {'5', {{std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0}}}},
         {'6', {{std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0}}}},
         {'7', {{std::byte{0xF0}, std::byte{0x10}, std::byte{0x20}, std::byte{0x40}, std::byte{0x40}}}},
         {'8', {{std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0}}}},
         {'9', {{std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0}, std::byte{0x10}, std::byte{0xF0}}}},
         {'A', {{std::byte{0xF0}, std::byte{0x90}, std::byte{0xF0}, std::byte{0x90}, std::byte{0x90}}}},
         {'B', {{std::byte{0xE0}, std::byte{0x90}, std::byte{0xE0}, std::byte{0x90}, std::byte{0xE0}}}},
         {'C', {{std::byte{0xF0}, std::byte{0x80}, std::byte{0x80}, std::byte{0x80}, std::byte{0xF0}}}},
         {'D', {{std::byte{0xE0}, std::byte{0x90}, std::byte{0x90}, std::byte{0x90}, std::byte{0xE0}}}},
         {'E', {{std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0}}}},
         {'F', {{std::byte{0xF0}, std::byte{0x80}, std::byte{0xF0}, std::byte{0x80}, std::byte{0x80}}}}
     }; 
}

namespace DisplayOpcodes {
    void disp_clear_00E0(void) {
        Renderer::clear_internal_display_state();

    }

    void disp_draw_DXYN(std::int8_t vXID, std::int8_t vYID, int height) {

    }
}

namespace Config {
    int get_res_field(const std::string field) {
        YAML::Node config = YAML::LoadFile(configFilePath);

        return config[resToken][field].as<int>();
    }

    int get_res_length() {
        return Config::get_res_field(resHeightToken);
    }

    int get_res_width() {
        return get_res_field(resWidthToken);
    }
}

namespace Renderer {
    // constexpr int resLength;
    // const int resWidth;

    namespace {
        Eigen::Matrix<int, internalStateHeight, internalStateWidth> internalDisplayState;

        const int resLength = Config::get_res_length();
        const int resWidth = Config::get_res_width();
    }

    void clear_internal_display_state() {
        internalDisplayState.setZero();
    }

    void init_interal_game_state() {
        internalDisplayState.setZero();
    }

    void display_current_state_on_screen() {
    }
}