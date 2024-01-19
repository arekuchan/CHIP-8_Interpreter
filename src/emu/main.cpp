#include "main.hpp"

int main() {
    // TODO : set up SDL init
    try {
        Randomiser::init_randomiser();
        Registers::do_all_register_inits();
        Chip8VMRam::initalise_interpreter_memory();

        RenderEngine::init_render_engine();

    } catch (std::exception& e) {
        std::cout << "Exception: " << e.what();
        return 1;
    }

    std::cout << "Konnichi wa oniichan!\n";

    return 0;
}