#include "registers.hpp"

namespace Registers {
    VarRegisterWord v0;
    VarRegisterWord v1;
    VarRegisterWord v2;
    VarRegisterWord v3;

    VarRegisterWord v4;
    VarRegisterWord v5;
    VarRegisterWord v6;
    VarRegisterWord v7;

    VarRegisterWord v8;
    VarRegisterWord v9;
    VarRegisterWord vA;
    VarRegisterWord vB;

    VarRegisterWord vC;
    VarRegisterWord vD;
    VarRegisterWord vE;
    VarRegisterWord vF;

    // 16 bit memory address register
    // TODO: this should be std::uint16_t
    std::int16_t iRegister;

    // Program Counter
    std::uint16_t pcRegister{programStartAddr};

    // Call Stack
    std::array<uint16_t, maxCallStackSize> callStack;

    // Top of Stack (Position)
    // init with -1 to work with call_2nnn and ret_00EE
    int stackPointer{-1};

    // register identifies are 4 bits but lowest size is 8 bits
    const std::unordered_map<std::int8_t, std::int8_t&> variableRegistersMap{
        {0x0, v0}, {0x1, v1}, {0x2, v2}, {0x3, v3}, {0x4, v4}, {0x5, v5}, {0x6, v6}, {0x7, v7}, {0x8, v8},
        {0x9, v9}, {0xA, vA}, {0xB, vB}, {0xC, vC}, {0xD, vD}, {0xE, vE}, {0xF, vF}
    };

    void do_all_register_inits(void) {
        init_var_registers();
    }

    // used registers array, so make sure that is initialised first
    // sets all the registers to 0 bit pattern
    void init_var_registers(void) {
        for (int i = 0; i < numVarRegisters; i++) {
            *(Registers::varRegisters[i]) = 0;
        }
    }

    bool msb_is_set(std::byte someByte) {
        if ((someByte & std::byte{0x80}) == std::byte{0x80}) {
            return true;
        }

        return false;
    }

    std::unique_ptr<Sprites::SpriteMatrix> get_sprite(unsigned int height) {
        // yes passing Sprites::spriteWidth should be redundant but compiler-chan gets
        // very mad if you dont
        auto spriteMatrix = std::make_unique<Sprites::SpriteMatrix>(height, Sprites::spriteWidth);
        auto currRowMemLoc = get_i_register_val();

        for (int i : std::views::iota(0, static_cast<int>(height))) {
            std::byte rowByte = Chip8VMRam::read_byte_from_ram(currRowMemLoc);

            for (int j : std::views::iota(0, Sprites::spriteWidth)) {
                (*spriteMatrix)(i, j) = msb_is_set(rowByte) ? 1 : 0;
                rowByte <<= 1;
            }

            currRowMemLoc += 1;
        }

        return spriteMatrix;
    }

    int8_t& get_carry_flag_register(void) {
        auto& registerMap = Registers::variableRegistersMap;
        
        return registerMap.at(Registers::carryFlagRegisterNum);
    }

    std::tuple<int8_t&, int8_t&> get_vx_and_vy(std::int8_t vXAddr, std::int8_t vYAddr) {
        auto& registerMap = Registers::variableRegistersMap;

        std::int8_t& vX = registerMap.at(vXAddr);
        std::int8_t& vY = registerMap.at(vYAddr);

        return std::tuple<int8_t&, int8_t&>{vX, vY};
    }

    std::int16_t get_i_register_val(void) {
        return Registers::iRegister;
    }

    void set_i_register(int16_t bitPttrn) {
        iRegister = bitPttrn;
    }

    std::uint16_t get_pc_register_val(void) {
        return pcRegister;
    }

    void set_pc_register(std::uint16_t val) {
        pcRegister = val;
    }

    // also updates stack pointer
    void push_onto_call_stack(std::uint16_t addr) {
        stackPointer++;
        callStack[stackPointer] = addr;
    }

    // also updates stack pointer
    std::uint16_t pop_from_call_stack(void) {
        auto addr = callStack[stackPointer];
        stackPointer--;

        return addr;
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