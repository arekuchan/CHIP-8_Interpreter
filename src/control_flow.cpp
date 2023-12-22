#include "control_flow.hpp"

// TODO: all opcode functions will be changed to bool functions
// the bool returned will indicate whether the PC register has been changed
// from the execution of the instruction (true if has, else false)
// if it hasn't just proceed as normal with +1 to next instruction
// else, don't change the program counter register

// essentially the overall architecture and process should be like:
// *** parser reads program counter and fetches current instruction ***
// *** parser decodes current instruction and extracts its args ***
// *** instruction is executed and returns bool ***
// *** if true, don't update program counter, else PC += 1 ***
// *** go back to the first step ***

void ret_00EE(void) {
    Registers::set_pc_register(Registers::pop_from_call_stack());
}

void jmp_1nnn(std::int16_t addr) {
    Registers::set_pc_register(addr);
}

void call_2nnn(std::int16_t addr) {
    Registers::push_onto_call_stack(Registers::get_pc_register_val());
    Registers::set_pc_register(addr);
}

// generalised function for se_3xkk and sne_3xkk
// gets the val in var register x and compares it to data 
// using the condition function
// if the condition is met, skip the next instruction
// true is retured iff the next instruction is skipped (pc register is modifed)
// if this happens, don't modify the pc register directly after, just jump to the next instruction
// pointed currently by the PC register
bool skip_next_instrc_if_condition(std::int8_t vXID, std::int8_t data, auto condition) {
    auto& vXRegister = Registers::variableRegistersMap.at(vXID);

    if (condition(vXRegister, data)) {
        auto currPCVal = Registers::get_pc_register_val();

        if (currPCVal + 2 > Registers::maxPCRegNum) {
            throw RegisterOverflowException("Variable Register: " + int8ToHexString(vXID));
        }

        Registers::set_pc_register(currPCVal + 2);
        
        return true;
    }

    return false;
}

bool se_3xkk(std::int8_t vXID, std::int8_t data) {
    return skip_next_instrc_if_condition(vXID, data, ControlFlow::equalsCondition);
}

bool sne_4xkk(std::int8_t vXID, std::int8_t data) {
    return skip_next_instrc_if_condition(vXID, data, ControlFlow::notEqualsCondition);
}

bool se_5xy0(std::int8_t vXID, std::int8_t vYID) {
    std::int8_t data = Registers::variableRegistersMap.at(vYID);
    
    return skip_next_instrc_if_condition(vXID, data, ControlFlow::equalsCondition);
}

bool sne_9xy0(std::int8_t vXID, std::int8_t vYID) {
    std::int8_t data = Registers::variableRegistersMap.at(vYID);

    return skip_next_instrc_if_condition(vXID, data, ControlFlow::notEqualsCondition);
}