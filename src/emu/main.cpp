#include "main.hpp"

int main() {
    try {
        Randomiser::init_randomiser();
        Registers::do_all_register_inits();
        Chip8VMRam::initalise_interpreter_memory();
    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what();
    }

    std::cout << "Konnichi wa oniichan!\n";

    return 0;
}