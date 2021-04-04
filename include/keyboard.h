#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <array>
#include <cstdint>

class Keyboard {
    std::array<bool, 16> keys{{}};

public:
    void set_key_down(uint8_t key);
    void set_key_up(uint8_t key);
    bool is_key_down(uint8_t key);
    uint8_t is_key_down();
};

#endif //KEYBOARD_H
