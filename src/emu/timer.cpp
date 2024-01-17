#include "timer.hpp"

Chip8Timer::Chip8Timer(uint hertz, VarRegisterWord initVal, bool playSound) : 
            hertz(hertz), timerVal(initVal), playSound(playSound) {
            }