#include "main.hpp"

int main() {
    try {
        SDL_Init(SDL_INIT_AUDIO | SDL_INIT_VIDEO); // TODO : this needs better error handling

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