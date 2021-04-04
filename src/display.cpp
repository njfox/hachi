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
            if (x >= buffer_width) { x = 0; }
            if (y >= buffer_height) { y = 0; }
            pixel = &pixel_buffer.at(x).at(y);
            bool bit_set = byte & (1 << j);
            if (*pixel == 0x01 && bit_set) {
                collision = true;
            }
            *pixel = *pixel ^ bit_set; // XOR with j'th bit
            x++;
        }
        x = x_start;
        y++;
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

PixelBuffer* Display::get_pixel_buffer() {
    return &pixel_buffer;
}
