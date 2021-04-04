#include "include/keyboard.h"

#include <cstdint>

void Keyboard::set_key_down(uint8_t key) {
    keys.at(key) = 1;
}

void Keyboard::set_key_up(uint8_t key) {
    keys.at(key) = 0;
}

bool Keyboard::is_key_down(uint8_t key) {
    return keys.at(key);
}

uint8_t Keyboard::is_key_down() {
    for (auto key : keys) {
        if (is_key_down(key)) { return key; }
    }
    return 0xFF;
}
