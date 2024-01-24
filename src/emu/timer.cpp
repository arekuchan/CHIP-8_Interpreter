#include "timer.hpp"

Chip8Timer::Chip8Timer(uint hertz, VarRegisterWord initVal, bool playSound) : 
            hertz(hertz), timerVal(initVal), playSound(playSound) {
}

void Chip8Timer::beep(void) {
    if (!SDL_QueueAudio(this->deviceID, this->audioBuf, this->audioLen)) {
        throw SDLQueueAudioException(SDL_GetError());
    }

    // this function can toggle audio playblack, ironically
    SDL_PauseAudioDevice(this->deviceID, 0); // 0 means play
}

Chip8SoundTimer::Chip8SoundTimer(uint hertz, VarRegisterWord initVal) : Chip8Timer(hertz, initVal, true) {
}

Chip8DelayTimer::Chip8DelayTimer(uint hertz, VarRegisterWord initVal) : Chip8Timer(hertz, initVal, false) {
}

void TimerOps::set_timer(std::int8_t vXID, Chip8Timer& timer) {
        Registers::VarRegisterWord vXVal = Registers::variableRegistersMap.at(vXID);

        timer.set_val(vXVal);
    }

void TimerOps::set_delay_timer_FX15(std::int8_t vXID) {
        set_timer(vXID, Timers::delayTimer);
    }

void TimerOps::set_sound_timer_FX18(std::int8_t vXID) {
    set_timer(vXID, Timers::soundTimer);
}

void TimerOps::set_vx_to_delay_timer_FX07(std::int8_t vXID) {
    int8_t& vX = Registers::variableRegistersMap.at(vXID);

    vX = Timers::delayTimer.get_val();
}