#include "ram.hpp"

namespace Ram {
    // this is in bytes
    constexpr int amtInterpreterMem = 512;

    std::array<std::byte, amtInterpreterMem> interpreterMem{};

    // TODO : this function
    void initalise_interpreter_memory(void) {

    }
}