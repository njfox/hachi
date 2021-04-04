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
    memcpy(&ram.at(addr), &default_sprites, sizeof(default_sprites));
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

void Chip8::on_key_down(uint8_t key) {
    keyboard.set_key_down(key);
}

void Chip8::on_key_up(uint8_t key) {
    keyboard.set_key_up(key);
}

void Chip8::reboot() {
    ram.fill(0);
    stack.fill(0);
    display = Display{};
    keyboard = Keyboard{};
    cpu = Cpu{ clock_speed, &display, &keyboard, &ram, &stack };
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

const Keyboard& Chip8::get_keyboard() const {
    return keyboard;
}

uint64_t Chip8::get_clock_speed() {
    return clock_speed;
}
