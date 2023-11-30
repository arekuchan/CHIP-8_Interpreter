#include "ALU-Operations.h"
#include "registers.h"

// TODO : move this opcode to another file, it isn't an ALU operation
void assign_8XY0(std::int8_t vXAddr, std::int8_t vYAddr) {
    auto& registerMap = Registers::registerMap;

    registerMap.at(vXAddr) = registerMap.at(vYAddr);
}

void or_8XY1(std::int8_t vXAddr, std::int8_t vYAddr) {
    auto& registerMap = Registers::registerMap;

    registerMap.at(vXAddr) |= registerMap.at(vYAddr);
}

void and_8XY2(std::int8_t vXAddr, std::int8_t vYAddr) {
    auto& registerMap = Registers::registerMap;

    registerMap.at(vXAddr) &= registerMap.at(vYAddr);
}

void xor_8XY3(std::int8_t vXAddr, std::int8_t vYAddr) {
    auto& registerMap = Registers::registerMap;

    registerMap.at(vXAddr) ^= registerMap.at(vYAddr);
}

// addr is 4 bits in opcode, but min int size is 8 bits in c++ stdlib
void add_8XY4(std::int8_t vXAddr, std::int8_t vYAddr) {
    int8_t& carryFlagRegister = Registers::get_carry_flag_register(); 
    auto&& [vX, vY] = Registers::get_vx_and_vy(vXAddr, vYAddr);

    // perform addition and cast to 16 bits (to detect carry out from MSB)
    int16_t additionResult = vX + vY;

    // detect carry out by checking if 9th bit (left from MSB in 8 bit) is set
    bool carryOut = (0x1 << (9 - 1)) & additionResult;
    carryFlagRegister = carryOut ? 0x01 : 0x00;

    // the top half should get truncated
    vX = (int8_t) additionResult;
}

void minus_8XY5(std::int8_t vXAddr, std::int8_t vYAddr) {
    int8_t& carryFlagRegister = Registers::get_carry_flag_register();
    auto&& [vX, vY] = Registers::get_vx_and_vy(vXAddr, vYAddr);

    carryFlagRegister = vX > vY ? 0x01 : 0x00;
    vX -= vY;
}

void shr_8XY6(std::int8_t vXAddr, std::int8_t vYAddr) {
    int8_t& carryFlagRegister = Registers::get_carry_flag_register();
    auto&& vX = Registers::registerMap.at(vXAddr);

    carryFlagRegister = vX & 0x01 ? 0x01 : 0x00;
    vX >>= 1;
}

void subn_8XY7(std::int8_t vXAddr, std::int8_t vYAddr) {
    int8_t& carryFlagRegister = Registers::get_carry_flag_register();
    auto&& [vX, vY] = Registers::get_vx_and_vy(vXAddr, vYAddr);

    carryFlagRegister = vY > vX ? 0x01 : 0x00;
    vX = vY - vX;
}

void shl_8XYE(std::int8_t vXAddr, std::int8_t vYAddr) {
    int8_t& carryFlagRegister = Registers::get_carry_flag_register();
    auto&& vX = Registers::registerMap.at(vXAddr);

    carryFlagRegister = vX & 0x80 ? 0x01 : 0x00;
    vX <<= 1;
}