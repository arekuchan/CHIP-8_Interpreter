#ifndef CHIP_8_ALU_OPERATIONS_H

#define CHIP_8_ALU_OPERATIONS_H
#include <cstdint>
#include "OpcodeType.hpp"

void addi_7XNN(std::int8_t, std::int8_t);

void assign_8XY0(std::int8_t, std::int8_t);

void or_8XY1(std::int8_t, std::int8_t);

void and_8XY2(std::int8_t, std::int8_t);

void xor_8XY3(std::int8_t, std::int8_t);

void add_8XY4(std::int8_t, std::int8_t);

void minus_8XY5(std::int8_t, std::int8_t);

void shr_8XY6(std::int8_t, std::int8_t);

void subn_8XY7(std::int8_t, std::int8_t);

void shl_8XYE(std::int8_t, std::int8_t);

#endif