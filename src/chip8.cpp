#include "include/chip8.h"

#include <chrono>
#include <cstring>
#include <fstream>
#include <thread>

void Chip8::load_rom(const string filename) {
    std::ifstream input(filename, std::ios::in|std::ios::binary);
    char* out = reinterpret_cast<char*>(&ram[0x200]);
    input.read(out, 0xDFF);
    input.close();
}

void Chip8::load_default_sprites(uint16_t addr) {
    memcpy(&ram, &default_sprites, sizeof(default_sprites));
}

void Chip8::start() {
    using namespace::std::chrono;
    load_default_sprites(0);
    while (true) {
        cpu.decrement_delay();
        cpu.decrement_sound();

        auto next = std::chrono::steady_clock::now() + 16.6667ms;
        for (size_t i{}; i < clock_speed / 60; i++) {
            cpu.execute();
        }
        std::this_thread::sleep_until(next);
    }
}

void Chip8::execute_cycle() {
    cpu.execute();
}

bool Chip8::check_sound() {
    return cpu.get_sound();
}

bool Chip8::check_delay() {
    return cpu.get_delay();
}

bool Chip8::decrement_delay() {
    return cpu.decrement_delay();
}

bool Chip8::decrement_sound() {
    return cpu.decrement_sound();
}

Display* Chip8::get_display() {
    return &display;
}

uint64_t Chip8::get_clock_speed() {
    return clock_speed;
}
