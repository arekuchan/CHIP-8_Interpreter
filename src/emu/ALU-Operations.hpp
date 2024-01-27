#ifndef CHIP_8_ALU_OPERATIONS_H
#define CHIP_8_ALU_OPERATIONS_H

#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <ranges>

#include "OpcodeType.hpp"
#include "ram.hpp"

namespace Randomiser {
    void init_randomiser(void);

    int8_t rand_8_bit_num(void);
}

// TODO: migrate whole codebase to use unsigned ints
// just learned only now that chip-8 registers are unsigned...

namespace ALU_Ops {
    using std::views::reverse;
    using std::views::iota;

    bool ldi_6XNN(std::int8_t, std::int8_t);

    void addi_7XNN(std::int8_t, std::int8_t);

    void assign_8XY0(std::int8_t, std::int8_t);

    void or_8XY1(std::int8_t, std::int8_t);

    void and_8XY2(std::int8_t, std::int8_t);

    void xor_8XY3(std::int8_t, std::int8_t);

    void add_8XY4(std::uint8_t, std::uint8_t);

    void minus_8XY5(std::int8_t, std::int8_t);

    void shr_8XY6(std::int8_t, std::int8_t);

    void subn_8XY7(std::int8_t, std::int8_t);

    void shl_8XYE(std::int8_t, std::int8_t);

    void add_to_i_FX1E(std::int8_t);

    bool ld_rand_CXNN(std::int8_t, std::int8_t);

    void set_bcd_fx33(std::int8_t);
}

#endif
