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
        RenderEngine::reset_display();
    }

    void disp_draw_DXYN(std::int8_t xCoord, std::int8_t yCoord, std::uint8_t height) {
        std::unique_ptr<Sprites::SpriteMatrix> spriteMatrix = Registers::get_sprite(height);

        RenderEngine::xor_sprite_on_screen(*spriteMatrix, xCoord, yCoord);
    }
}

namespace Config {
    int get_res_field(const std::string field) {
        YAML::Node config = YAML::LoadFile(configFilePath);

        return config[resToken][field].as<int>();
    }

    int get_res_height() {
        return Config::get_res_field(resHeightToken);
    }

    int get_res_width() {
        return get_res_field(resWidthToken);
    }
}

namespace RenderEngine {
    // constexpr int resLength;
    // const int resWidth;

    namespace {
        // each cell represents a pixel
        Eigen::Matrix<int, internalStateHeight, internalStateWidth> internalDisplayState;

        // given user configs for res, the render engine will try best
        // fit them to be compatible with the internal game state

        const int resHeight = adjust_res_height(Config::get_res_height());
        const int resWidth = adjust_res_width(Config::get_res_width());

        // to scale up the sprites/screen, each internal cell is scaled up such that
        // the cells dimensions are multiplied by the appropriate scale
        const int resHeightScale = resHeight / internalStateHeight; 
        const int resWidthScale = resWidth / internalStateWidth;

        const char* windowName = "Chip 8 Emulator";

        SDL_Renderer* renderer;
        SDL_Window* window;

        void clear_internal_display_state() {
            internalDisplayState.setZero();
        }

        void init_interal_game_state() {
            internalDisplayState.setZero();
        }

        int get_internal_disp_cell(int row, int col) {
            return internalDisplayState(row, col);
        }

        void set_internal_disp_cell(int row, int col, int val) {
            internalDisplayState(row, col) = val;
        }
    }

    // isHeight == true if height is being passed, else false if width is being passed
    int adjust_res_dimension(int resDimLen, bool isHeight) {
        int interalDimLen = isHeight ? internalStateHeight : internalStateWidth;

        if (resDimLen < interalDimLen) {
            return interalDimLen;
        } else if (resDimLen % interalDimLen != 0) {
            int scale = resDimLen / interalDimLen;

            return interalDimLen * scale;
        }

        return resDimLen;
    }

    int adjust_res_height(int resHeight) {
        return adjust_res_dimension(resHeight, true);
    }

    int adjust_res_width(int resWidth) {
        return adjust_res_dimension(resWidth, false);
    }

    void init_render_engine(void) {
        window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 
            resWidth, resHeight, SDL_WINDOW_SHOWN);

        if (!window) {
            throw RenderEngineWindowCreationException();
        }

        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
        renderer = renderer ? renderer : SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);

        if (!renderer) {
            // can't make a software nor hardware based renderer
            throw RenderEngineRendererCreationException();
        }

        init_interal_game_state();
    }

    void reset_display() {
        clear_internal_display_state();

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE); // pitch black
        SDL_RenderClear(renderer);
    }

    void set_sdl_rect(SDL_Rect* rect, int x, int y, int w, int h) {
        rect->x = x;
        rect->y = y;
        rect->w = w;
        rect->h = h;
    }

    void display_current_state_on_screen() {
        for (int i : std::views::iota(0, internalStateHeight)) {
            for (int j : std::views::iota(0, internalStateWidth)) {
                SDL_Rect rect;
                set_sdl_rect(&rect, i * resWidthScale, j * resHeightScale, 
                    resWidthScale, resHeightScale);

                if (internalDisplayState(i, j) == 1) {
                    // white
                    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
                } else {
                    SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
                }

                SDL_RenderFillRect(renderer, &rect);
                SDL_RenderPresent(renderer);
            }
        }
    }

    void xor_sprite_on_screen(const Sprites::SpriteMatrix& sprite, int spriteXCoord, int spriteYCoord) {
        int spriteHeight = sprite.rows();
        int spriteWidth = sprite.cols();

        for (int i : std::views::iota(0, spriteHeight)) {
            // modulo for bounds and wrap around functionality
            int dispRowCoord = (spriteYCoord + i) % internalStateHeight;

            for (int j : std::views::iota(0, spriteWidth)) {
                int dispColCoord = (spriteXCoord + j) % internalStateWidth;

                int currCell = get_internal_disp_cell(dispRowCoord, dispColCoord);
                int spriteCell = sprite(i, j);

                int xorVal = Misc::logical_xor(currCell, spriteCell);

                set_internal_disp_cell(dispRowCoord, dispColCoord, xorVal);
            }
        }
    }
}