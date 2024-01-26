#ifndef CHIP_8_CONTROL_FLOW

#define CHIP_8_CONTROL_FLOW

#include <cstdint>
#include <unordered_map>
#include <cctype>

#include <SDL2/SDL.h>

#include "registers.hpp"
#include "exceptions.hpp"
#include "parser.hpp"

namespace Conditions {
    const auto notEquals = []<typename T, typename U> (T a, U b) {return a != b; };
    const auto equals = []<typename T, typename U> (T a, U b) { return a == b; };   
}

namespace ControlFlowOps {
    bool ret_00EE(void);

    void jmp_1nnn(std::int16_t);

    void call_2nnn(std::int16_t);

    bool se_3xkk(std::int8_t, std::int8_t);

    bool sne_4xkk(std::int8_t, std::int8_t);

    bool se_5xy0(std::int8_t, std::int8_t);

    bool sne_9xy0(std::int8_t, std::int8_t);

    bool jmp_BNNN(std::int16_t);

    bool get_key_fx0a(std::int8_t);

    bool skip_if_key_ex9e(std::int8_t);

    bool skip_if_not_key_exa1(std::int8_t);
}

#endif
