#include <cstdint>

#include "include/display.h"

bool Display::draw(uint8_t x, uint8_t y, uint8_t* sprite, int8_t size) {
    bool collision{ false };
    uint8_t* pixel{};
    uint8_t byte{};
    uint8_t x_start = x;
    uint8_t buffer_width = pixel_buffer.size();
    uint8_t buffer_height = pixel_buffer.at(0).size();
    for (int8_t i{}; i < size; i++) {
        byte = *sprite;
        for (int8_t j{7}; j >= 0; j--) {
            pixel = &pixel_buffer.at(x % buffer_width).at((y + i) % buffer_height);
            bool bit_set = byte & (1 << j);
            if (*pixel && bit_set) {
                collision = true;
            }
            *pixel ^= bit_set; // XOR with j'th bit
            x++;
        }
        x = x_start;
        sprite++;
    }
    return collision;
}

void Display::clear_screen() {
    for (size_t i{}; i < pixel_buffer.size(); i++) {
        for (size_t j{}; j < pixel_buffer.at(0).size(); j++) {
            pixel_buffer.at(i).at(j) = 0;
        }
    }
}

const PixelBuffer& Display::get_pixel_buffer() const {
    return pixel_buffer;
}
