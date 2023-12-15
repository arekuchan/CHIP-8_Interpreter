#include "control_flow.hpp"

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