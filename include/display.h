#ifndef DISPLAY_H
#define DISPLAY_H

#include <array>
#include <cstdint>

using PixelBuffer = std::array<std::array<uint8_t, 32>, 64>;

class Display {
    PixelBuffer pixel_buffer{{}};
public:
    Display(){};
    bool draw(uint8_t x, uint8_t y, uint8_t* sprite, int8_t size);
    void clear_screen();
    const PixelBuffer& get_pixel_buffer() const;
};

#endif // DISPLAY_H
