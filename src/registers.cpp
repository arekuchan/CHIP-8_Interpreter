#include "registers.hpp"

namespace Registers {
    int8_t v0;
    int8_t v1;
    int8_t v2;
    int8_t v3;

    int8_t v4;
    int8_t v5;
    int8_t v6;
    int8_t v7;

    int8_t v8;
    int8_t v9;
    int8_t vA;
    int8_t vB;

    int8_t vC;
    int8_t vD;
    int8_t vE;
    int8_t vF;

    // 16 bit memory address register
    int16_t iRegister;

    // register identifies are 4 bits but lowest size is 8 bits
    const std::unordered_map<std::int8_t, std::int8_t&> variableRegistersMap{
        {0x0, v0}, {0x1, v1}, {0x2, v2}, {0x3, v3}, {0x4, v4}, {0x5, v5}, {0x6, v6}, {0x7, v7}, {0x8, v8},
        {0x9, v9}, {0xA, vA}, {0xB, vB}, {0xC, vC}, {0xD, vD}, {0xE, vE}, {0xF, vF}
    };

    void do_all_register_inits(void) {
        init_registers();
    }

    // used registers array, so make sure that is initialised first
    // sets all the registers to 0 bit pattern
    void init_registers(void) {
        for (int i = 0; i < numRegisters; i++) {
            *(registers[i]) = 0;
        }
    }

    int8_t& get_carry_flag_register(void) {
        auto& registerMap = Registers::variableRegistersMap;
        
        return registerMap.at(Registers::carryFlagRegisterNum);
    }

    std::tuple<int8_t&, int8_t&> get_vx_and_vy(std::int8_t vXAddr, std::int8_t vYAddr) {
        auto& registerMap = Registers::variableRegistersMap;

        int8_t& vX = registerMap.at(vXAddr);
        int8_t& vY = registerMap.at(vYAddr);

        return std::tuple<int8_t&, int8_t&>{vX, vY};
    }

    int16_t& get_i_register(void) {
        return Registers::iRegister;
    }

    void set_i_register(int16_t bitPttrn) {
        iRegister = bitPttrn;
    }

    bool i_register_wont_overflow(int num) {
        int iRegisterSize = Registers::iRegisterSize;
        int lower = -1 * std::pow(2, iRegisterSize - 1);
        int upper = std::pow(2, iRegisterSize - 1) - 1;

        if (lower <= num && num <= upper) {
            return true;
        }

        return false;
    }
}