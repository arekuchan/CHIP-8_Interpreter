#include "ALU-Operations.h"
#include "registers.h"

// addr is 4 bits in opcode, but min int size is 8 bits in c++ stdlib
void add_8XY4(std::int8_t vXAddr, std::int8_t vYAddr) {
    // get values in registers
    auto& registerMap = Registers::registerMap;
    int8_t& carryFlagRegister = registerMap.at(Registers::carryFlagRegisterNum);

    // perform addition and cast to 16 bits (to detect carry out from MSB)
    int16_t additionResult = registerMap.at(vXAddr) + registerMap.at(vYAddr);

    // detect carry out by checking if 9th bit (left from MSB in 8 bit) is set
    bool carryOut = (0x1 << (9 - 1)) & additionResult;

    if (carryOut) {
        carryFlagRegister = 0x1;
    } else {
        carryFlagRegister = 0x0;
    }

    // the top half should get truncated
    registerMap.at(vXAddr) = (int8_t) additionResult;
}