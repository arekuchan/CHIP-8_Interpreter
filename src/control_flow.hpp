#ifndef CHIP_8_CONTROL_FLOW

#define CHIP_8_CONTROL_FLOW

#include <cstdint>
#include "registers.hpp"
#include "exceptions.hpp"

void ret_00EE(void);

void jmp_1nnn(std::int16_t);

void call_2nnn(std::int16_t);

bool se_3xkk(std::int8_t, std::int8_t);

#endif