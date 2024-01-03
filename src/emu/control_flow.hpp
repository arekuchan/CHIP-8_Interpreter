#ifndef CHIP_8_CONTROL_FLOW

#define CHIP_8_CONTROL_FLOW

#include <cstdint>
#include "registers.hpp"
#include "exceptions.hpp"

namespace ControlFlow {
    const auto notEqualsCondition = []<typename T, typename U> (T a, U b) {return a != b; };
    const auto equalsCondition = []<typename T, typename U> (T a, U b) { return a == b; };   
}

void ret_00EE(void);

void jmp_1nnn(std::int16_t);

void call_2nnn(std::int16_t);

bool skip_next_instrc_if_condition(std::int8_t, std::int8_t, auto);

bool se_3xkk(std::int8_t, std::int8_t);

bool sne_4xkk(std::int8_t, std::int8_t);

bool se_5xy0(std::int8_t, std::int8_t);

bool sne_9xy0(std::int8_t, std::int8_t);

bool jmp_BNNN(std::int16_t);

#endif