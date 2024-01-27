#include "ALU-Operations.hpp"
#include "registers.hpp"

// TODO : rename this file to ALU-OpCodes

namespace Randomiser {
    void init_randomiser(void) {
        std::srand(std::time(nullptr));
    }

    int8_t rand_8_bit_num(void) {
        // 8 bit range : [0, 255]
        // [0 - 128, 255 - 128] -> [-128, 127]
        return (rand() % 256) - 128;
    }
}

// TODO : change var names of vXAddr to vXID
// As they aren't addresses
namespace ALU_Ops {
    bool ldi_6XNN(std::int8_t vXID, std::int8_t constant) {
        auto& registerMap = Registers::variableRegistersMap;
        registerMap.at(vXID) = constant;

        return false;
    }

    void addi_7XNN(std::int8_t vXAddr, std::int8_t constant) {
        auto& registerMap = Registers::variableRegistersMap;

        registerMap.at(vXAddr) += constant;
    }

    // TODO : move this opcode to another file, it isn't an ALU operation
    void assign_8XY0(std::int8_t vXAddr, std::int8_t vYAddr) {
        auto& registerMap = Registers::variableRegistersMap;

        registerMap.at(vXAddr) = registerMap.at(vYAddr);
    }

    void or_8XY1(std::int8_t vXAddr, std::int8_t vYAddr) {
        auto& registerMap = Registers::variableRegistersMap;

        registerMap.at(vXAddr) |= registerMap.at(vYAddr);
    }

    void and_8XY2(std::int8_t vXAddr, std::int8_t vYAddr) {
        auto& registerMap = Registers::variableRegistersMap;

        registerMap.at(vXAddr) &= registerMap.at(vYAddr);
    }

    void xor_8XY3(std::int8_t vXAddr, std::int8_t vYAddr) {
        auto& registerMap = Registers::variableRegistersMap;

        registerMap.at(vXAddr) ^= registerMap.at(vYAddr);
    }

    // addr is 4 bits in opcode, but min int size is 8 bits in c++ stdlib
    void add_8XY4(std::uint8_t vXAddr, std::uint8_t vYAddr) {
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
        auto&& vX = Registers::variableRegistersMap.at(vXAddr);

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
        auto&& vX = Registers::variableRegistersMap.at(vXAddr);

        carryFlagRegister = vX & 0x80 ? 0x01 : 0x00;
        vX <<= 1;
    }

    void add_to_i_FX1E(std::int8_t vXAddr) {
        int16_t iRegisterVal = Registers::get_i_register_val();
        int8_t& vX = Registers::variableRegistersMap.at(vXAddr);

        Registers::set_i_register(iRegisterVal + vX);
    }

    bool ld_rand_CXNN(std::int8_t vXID, std::int8_t constant) {
        auto& vXRegister = Registers::variableRegistersMap.at(vXID);

        // apparently implementations typically make rand() have a range of [0, 255]
        // but I've been using 2s complement
        vXRegister = Randomiser::rand_8_bit_num() & constant;

        return false;
    }

    // could generalise this function, but chip 8 spec probably isn't changing anytime soon
    void set_bcd_fx33(std::int8_t vXID) {
        int onesDigitMemOffset = 2;
        int8_t vXVal = Registers::variableRegistersMap.at(vXID);

        for (int i : reverse(iota(0, onesDigitMemOffset))) {
            int8_t leastSignifDigit = vXVal % 10;
            int16_t loc = Registers::get_i_register_val() + i;

            Chip8VMRam::write_byte_to_ram(static_cast<std::byte>(leastSignifDigit), loc);

            vXVal /= 10;
        }
    }
}
