#ifndef CHIP8_H
#define CHIP8_H

#include "cpu.h"
#include "keyboard.h"

#include <array>
#include <cstdint>
#include <memory>
#include <string>

using std::string;
using std::array;

class Chip8 {
    array<uint8_t, 0x1000> ram{{}};
    array<uint16_t, 16> stack{{}};
    uint64_t clock_speed{540};
    Display display{};
    Keyboard keyboard{};
    Cpu cpu{clock_speed, &display, &keyboard, &ram, &stack};
    static constexpr array<uint8_t, 80> default_sprites{{ 0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
                                                          0x20, 0x60, 0x20, 0x20, 0x70, // 1
                                                          0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
                                                          0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
                                                          0x90, 0x90, 0xF0, 0x10, 0x10, // 4
                                                          0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
                                                          0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
                                                          0xF0, 0x10, 0x20, 0x40, 0x40, // 7
                                                          0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
                                                          0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
                                                          0xF0, 0x90, 0xF0, 0x90, 0x90, // A
                                                          0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
                                                          0xF0, 0x80, 0x80, 0x80, 0xF0, // C
                                                          0xE0, 0x90, 0x90, 0x90, 0xE0, // D
                                                          0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
                                                          0xF0, 0x80, 0xF0, 0x80, 0x80  // F
                                                       }};

public:
    Chip8(){};
    void load_rom(const string path);
    void load_default_sprites(uint16_t addr);
    void start();
    void execute_cycle();
    bool check_sound();
    bool check_delay();
    bool decrement_sound();
    bool decrement_delay();
    Display* get_display();
    Keyboard* get_keyboard();
    uint64_t get_clock_speed();
};

#endif // CHIP8_H
