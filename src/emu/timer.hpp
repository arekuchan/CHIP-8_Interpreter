#ifndef _CHIP_8_TIMER
#define _CHIP_8_TIMER

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <semaphore>
#include <thread>
#include <chrono>
#include <ratio>

#include <SDL2/SDL.h>

#include "registers.hpp"
#include "config.hpp"

// A timer class specifically for chip 8,
// not meant to be a general timer class
class Chip8Timer {
    private:
        inline static SDL_AudioSpec audioSpec;
        inline static Uint32 audioLen;
        inline static Uint8* audioBuf;

        inline static SDL_AudioDeviceID deviceID;

        friend class timerStaticConstr;

        class timerStaticConstr {
            timerStaticConstr() {
                auto beepFilePath = Config::get_beep_sound_path();

                if (!SDL_LoadWAV(beepFilePath.c_str(), &audioSpec, &audioBuf, &audioLen)) {
                    throw SDLLoadWavException(SDL_GetError());
                } else if (!(deviceID = SDL_OpenAudioDevice(NULL, 0, &audioSpec, NULL, 0))) {
                    throw SDLOpenAudioDevException(SDL_GetError());
                }
            }
        };

        static timerStaticConstr staticConstr;

    protected:
        using VarRegisterWord = Registers::VarRegisterWord;

        std::atomic<bool> exitTimer {false};

        bool timerAwake {false};
        std::mutex awakeMutex {};
        std::condition_variable awakeCV {};

        bool playSound;

        VarRegisterWord timerVal;

        const uint hertz;

        // std::chrono::duration<double> is in seconds, but allows decimal values
        // 60 hz is 60 cycles per second, so period of 1 cycle = (1 / 60) seconds
        const std::chrono::duration<double> timerDelay = (1 / hertz) * std::chrono::duration<double>();

        const std::jthread decThread {[this]() {
            std::unique_lock lk {awakeMutex};

            while (true) {
                awakeCV.wait(lk, [this]{ return timerAwake; });

                if (exitTimer) {
                    break;
                } else if (timerVal == 0) {
                    timerAwake = false;
                    continue;
                }

                if (playSound) { beep(); }
                timerVal--;

                lk.release();
                std::this_thread::sleep_for(timerDelay);
                lk.lock();
            }
        }};

        void beep(void);

    public:
        Chip8Timer(uint, VarRegisterWord, bool);

        void set_val(VarRegisterWord val) {
            std::unique_lock lk {awakeMutex};
            timerVal = val;
        }

        VarRegisterWord get_val(void) {
            std::unique_lock lk {awakeMutex};
            return timerVal;
        }

        void start_timer(void) {
            std::unique_lock lk {awakeMutex};
            timerAwake = true;
        }
};

class Chip8SoundTimer : public Chip8Timer {
    public:
        Chip8SoundTimer(uint, VarRegisterWord);
};

class Chip8DelayTimer : public Chip8Timer {
    public:
        Chip8DelayTimer(uint, VarRegisterWord);
};

namespace Timers {
    inline constexpr uint hertz = 60;
    inline constexpr Registers::VarRegisterWord initVal = 0x00;

    Chip8SoundTimer soundTimer{hertz, initVal};
    Chip8DelayTimer delayTimer{hertz, initVal};
}

namespace TimerOps {
    void set_timer(std::int8_t, Chip8Timer&); 

    void set_delay_timer_FX15(std::int8_t);

    void set_sound_timer_FX18(std::int8_t); 

    void set_vx_to_delay_timer_FX07(std::int8_t);
}

#endif