#include "control_flow.hpp"
#include "OpcodeType.hpp"

namespace ControlFlowOps {
    // TODO: all opcode functions (in all sourec files) will be changed to bool functions
    // the bool returned will indicate whether the PC register has been changed
    // from the execution of the instruction (true if has, else false)
    // if it hasn't just proceed as normal with +opcodeSize to next instruction
    // else, don't change the program counter register

    // essentially the overall architecture and process should be like:
    // *** parser reads program counter and fetches current instruction ***
    // *** parser decodes current instruction and extracts its args ***
    // *** instruction is executed and returns bool ***
    // *** if true, don't update program counter, else PC += opcodeSize ***
    // *** go back to the first step ***

    bool ret_00EE(void) {
        Registers::set_pc_register(Registers::pop_from_call_stack());
        return true;
    }

    bool jmp_1nnn(std::uint16_t addr) {
        Registers::set_pc_register(addr);
        return true;
    }

    void call_2nnn(std::uint16_t addr) {
        Registers::push_onto_call_stack(Registers::get_pc_register_val());
        Registers::set_pc_register(addr);

        return true;
    }

    void skip_next_instrc(void) {
        auto currPCVal = Registers::get_pc_register_val();

        if (currPCVal + 2 > Registers::maxPCRegNum) {
            throw RegisterOverflowException("Program Counter Register");
        }
        
        Registers::set_pc_register(currPCVal + opcodeSize);
    }

    // generalised function for se_3xkk and sne_3xkk
    // gets the val in var register x and compares it to data 
    // using the condition function
    // if the condition is met, skip the next instruction
    // true is retured iff the next instruction is skipped (pc register is modifed)
    bool skip_next_instrc_if_condition(std::int8_t vXID, std::int8_t data, auto condition) {
        auto& vXRegister = Registers::variableRegistersMap.at(vXID);

        if (condition(vXRegister, data)) {
            skip_next_instrc();
            return true;
        }

        return false;
    }

    bool se_3xkk(std::int8_t vXID, std::int8_t data) {
        return skip_next_instrc_if_condition(vXID, data, Conditions::equals);
    }

    bool sne_4xkk(std::int8_t vXID, std::int8_t data) {
        return skip_next_instrc_if_condition(vXID, data, Conditions::notEquals);
    }

    bool se_5xy0(std::int8_t vXID, std::int8_t vYID) {
        std::int8_t data = Registers::variableRegistersMap.at(vYID);

        return skip_next_instrc_if_condition(vXID, data, Conditions::equals);
    }

    bool sne_9xy0(std::int8_t vXID, std::int8_t vYID) {
        std::int8_t data = Registers::variableRegistersMap.at(vYID);

        return skip_next_instrc_if_condition(vXID, data, Conditions::notEquals);
    }

    bool jmp_BNNN(std::int16_t addr) {
        auto v0Val = Registers::variableRegistersMap.at(0x0);
        int32_t result = v0Val + addr;

        if (result > INT16_MAX) {
            throw RegisterOverflowException("PC register");
        }

        Registers::set_pc_register(result);

        return true;
    }

    const char* get_key_pressed(SDL_Keycode pressed) {
        return SDL_GetKeyName(pressed);
    }

    // hex keyboard is used with keys labelled from 0 to F
    // in memory the direct value is stored, i.e F -> 0x0F

    bool get_key_fx0a(std::int8_t vXID) {
        bool notWaitErr;
        SDL_Event event;

        while ((notWaitErr = SDL_WaitEvent(&event))) {
            if (event.type != SDL_KEYDOWN) {
                continue;
            }

            const char* key = get_key_pressed(event.key.keysym.sym);

            if (!Parser::is_hex_char(*key)) {
                // just ignore
                return false;
            }

            auto& vX = Registers::variableRegistersMap.at(vXID);
            vX = Parser::get_int_from_hex_char(*key);
            break;
        }

        if (!notWaitErr) {
            throw SDLWaitException(SDL_GetError());
        }

        return false;
    }

    int8_t get_pressed_key_num(SDL_Keycode pressdCode) {
        const char* pressedKey = get_key_pressed(pressdCode);

        return Parser::get_int_from_hex_char(*pressedKey);
    }

    // NOTE : not sure if this naive implementation of detecting key presses
    // will work, I'm unfamiliar with how SDL2 does event handling

    bool skip_if_key_ex9e(std::int8_t vXID) {
        SDL_Event event;

        SDL_PollEvent(&event);

        if (event.type != SDL_KEYDOWN) {
            return false;
        }

        int8_t pressdKeyNum = get_pressed_key_num(event.key.keysym.sym);

        return skip_next_instrc_if_condition(vXID, pressdKeyNum, Conditions::equals);
    }

    bool skip_if_not_key_exa1(std::int8_t vXID) {
        SDL_Event event;

        SDL_PollEvent(&event);

        if (event.type != SDL_KEYDOWN) {
            skip_next_instrc();
            return true;
        }

        int8_t pressdKeyNum = get_pressed_key_num(event.key.keysym.sym);

        return skip_next_instrc_if_condition(vXID, pressdKeyNum, Conditions::notEquals);
    }
}
