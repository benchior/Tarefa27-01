#include "ws2812.h"
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"

#define WS2812_PIO pio0
#define WS2812_SM 0

// Inicialização do WS2812
void ws2812_init(uint pin, uint num_leds) {
    uint offset = pio_add_program(WS2812_PIO, &ws2812_program);
    ws2812_program_init(WS2812_PIO, WS2812_SM, offset, pin, 800000, false);
}

// Define a cor de um pixel
void ws2812_set_pixel(uint32_t pixel_num, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = (r << 24) | (g << 16) | (b << 8);
    pio_sm_put_blocking(WS2812_PIO, WS2812_SM, color);
}

// Atualiza a matriz de LEDs
void ws2812_show() {
    pio_sm_put_blocking(WS2812_PIO, WS2812_SM, 0);
}