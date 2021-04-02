#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cstdint>

using PixelBuffer = std::array<std::array<int8_t, 32>, 64>;

class Display {
    PixelBuffer pixel_buffer{{}};
public:
    Display(){};
    bool draw(int8_t x, int8_t y, uint8_t* sprite, int8_t size);
    void clear_screen();
    PixelBuffer* get_pixel_buffer();
};

#endif // DISPLAY_H
