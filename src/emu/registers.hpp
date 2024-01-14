#ifndef CHIP_8_REGISTERS_H

#define CHIP_8_REGISTERS_H

#include <unordered_map>
#include <tuple>
#include <functional>

#include <cstdint>
#include <cmath>
#include <memory>

#include <Eigen/Dense>

#include "ram.hpp"

// registers
namespace Registers {
    constexpr int numVarRegisters = 16;
    constexpr std::int8_t carryFlagRegisterNum = 0xF;

    extern std::int8_t v0;
    extern std::int8_t v1;
    extern std::int8_t v2;
    extern std::int8_t v3;

    extern std::int8_t v4;
    extern std::int8_t v5;
    extern std::int8_t v6;
    extern std::int8_t v7;

    extern std::int8_t v8;
    extern std::int8_t v9;
    extern std::int8_t vA;
    extern std::int8_t vB;

    extern std::int8_t vC;
    extern std::int8_t vD;
    extern std::int8_t vE;
    extern std::int8_t vF;

    // size in bits
    inline constexpr int iRegisterSize = 16;

    inline constexpr uint16_t programStartAddr = 0x200;

    inline constexpr int pcRegisterSize = 16;

    inline constexpr int maxPCRegNum = 65535; // (2 ** pcRegisterSize) - 1

    inline constexpr int minPCRegNum = 0;

    // max num of elements
    inline constexpr int maxCallStackSize = 16;

    std::int8_t* const varRegisters[numVarRegisters] = {&v0, &v1, &v2, &v3, &v4, &v5, &v6, &v7, &v8, &v9, &vA, &vB, &vC, &vD, &vE, &vF};

    // map register number to a pointer to the register
    extern const std::unordered_map<int8_t, std::int8_t&> variableRegistersMap;

    void init_var_registers(void);

    void do_all_register_inits(void);

    std::int8_t& get_carry_flag_register(void);

    std::tuple<int8_t&, int8_t&> get_vx_and_vy(std::int8_t, std::int8_t);

    std::int16_t get_i_register_val(void);

    std::uint16_t get_pc_register_val(void);

    void set_pc_register(std::uint16_t);

    void set_i_register(std::int16_t);

    void push_onto_call_stack(std::uint16_t);

    std::uint16_t pop_from_call_stack(void);

    bool i_register_wont_overflow(int);

    bool msb_is_set(std::byte);

    std::unique_ptr<Sprites::SpriteMatrix> get_sprite(unsigned int);
}

#endif