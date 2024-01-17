#ifndef _CHIP_8_TIMER
#define _CHIP_8_TIMER

#include <atomic>
#include <mutex>
#include <condition_variable>

#include <semaphore>
#include <thread>
#include <chrono>
#include <ratio>

#include "registers.hpp"

// A timer class specifically for chip 8,
// not meant to be a general timer class
class Chip8Timer {
    using VarRegisterWord = Registers::VarRegisterWord;

    private:
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
            while (true) {
                std::unique_lock lk {awakeMutex};
                awakeCV.wait(lk, [this]{ return timerAwake; });

                if (exitTimer) {
                    break;
                } else if (timerVal == 0) {
                    timerAwake = false;
                    if (playSound) { beep(); }
                    continue;
                }

                timerVal--;
                std::this_thread::sleep_for(timerDelay);
            }
        }};

        void beep(void) {
            // TODO : finish this
        }

        void wait_for_timer_awake(void) {
            // TODO : finish this
        }

    public:
        Chip8Timer(uint, VarRegisterWord, bool);

        void set_val(VarRegisterWord val) {
            timerVal = val;
        }

        VarRegisterWord read_val(void) {
            return timerVal;
        }

        void start_timer(void) {
            timerAwake.release();
        }
};

#endif