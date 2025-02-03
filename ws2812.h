#ifndef WS2812_H
#define WS2812_H

#include "pico/stdlib.h"

void ws2812_init(uint pin, uint num_leds);
void ws2812_set_pixel(uint32_t pixel_num, uint8_t r, uint8_t g, uint8_t b);
void ws2812_show();

#endif