#ifndef CHIP_8_REGISTERS_H

#define CHIP_8_REGISTERS_H

#include <unordered_map>
#include <cstdint>

// registers
namespace Registers {
    constexpr int numRegisters = 16;
    constexpr int8_t carryFlagRegisterNum = 0xF;

    extern int8_t v0;
    extern int8_t v1;
    extern int8_t v2;
    extern int8_t v3;

    extern int8_t v4;
    extern int8_t v5;
    extern int8_t v6;
    extern int8_t v7;

    extern int8_t v8;
    extern int8_t v9;
    extern int8_t vA;
    extern int8_t vB;

    extern int8_t vC;
    extern int8_t vD;
    extern int8_t vE;
    extern int8_t vF;

    int8_t* const registers[numRegisters] = {&v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &vA, &vB, &vC, &vD, &vE, &vF};

    // map register number to a pointer to the register
    extern const std::unordered_map<int8_t, int8_t&> registerMap;

    void init_registers(void);

    void do_all_register_inits(void);
}

#endif