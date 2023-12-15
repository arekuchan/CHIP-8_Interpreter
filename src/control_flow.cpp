#include "control_flow.hpp"

void jmp_1nnn(std::int16_t addr) {
    Registers::set_pc_register(addr);
}