#ifndef CHIP_8_CONTROL_FLOW

#define CHIP_8_CONTROL_FLOW

#include <cstdint>
#include "registers.hpp"

void ret_00EE(void);

void jmp_1nnn(std::int16_t);

void call_2nnn(std::int16_t);

#endif