#include "include/hachi.h"

#include <chrono>

void Hachi::run() {
    using namespace::std::chrono;
    chip8.load_default_sprites(0);
    chip8.load_rom(filename);
    auto clock_speed = chip8.get_clock_speed();
    while (true) {
        if (chip8.decrement_sound()) { emit beep(); }
        chip8.decrement_delay();
        emit refresh_screen(chip8.get_display()->get_pixel_buffer());

        auto next = std::chrono::steady_clock::now() + 16.6667ms;
        for (size_t i{}; i < clock_speed / 60; i++) {
            chip8.execute_cycle();
        }
        std::this_thread::sleep_until(next);
        if (currentThread()->isInterruptionRequested()) {
            return;
        }
    }
}

void Hachi::keyPressEvent(QKeyEvent *e) {
    auto foo = e->key();
}

