#include "main.hpp"

int main() {
    Registers::do_all_register_inits();
    InterpretersAllocatedRam::initalise_interpreter_memory();

    std::cout << "Konnichi wa oniichan!\n";

    return 0;
}