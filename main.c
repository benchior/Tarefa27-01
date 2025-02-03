#include "pico/stdlib.h"
#include "hardware/pwm.h"
#include "hardware/gpio.h"
#include "ws2812.h"

// Definições dos pinos
#define LED_RGB_RED_PIN 11
#define LED_RGB_GREEN_PIN 12
#define LED_RGB_BLUE_PIN 13
#define BUTTON_A_PIN 5
#define BUTTON_B_PIN 6
#define WS2812_PIN 7
#define NUM_LEDS 25

// Variáveis globais
volatile int current_number = 0;
volatile uint32_t last_button_press = 0;
const uint32_t DEBOUNCE_TIME = 200; // Tempo de debounce em ms

// Protótipos das funções
void blink_red_led();
void display_number(int number);
void button_a_irq(uint gpio, uint32_t events);
void button_b_irq(uint gpio, uint32_t events);

// Função principal
int main() {
    // Inicialização do hardware
    stdio_init_all();

    // Configuração do LED RGB
    gpio_set_function(LED_RGB_RED_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_RGB_GREEN_PIN, GPIO_FUNC_PWM);
    gpio_set_function(LED_RGB_BLUE_PIN, GPIO_FUNC_PWM);

    uint slice_red = pwm_gpio_to_slice_num(LED_RGB_RED_PIN);
    uint slice_green = pwm_gpio_to_slice_num(LED_RGB_GREEN_PIN);
    uint slice_blue = pwm_gpio_to_slice_num(LED_RGB_BLUE_PIN);

    pwm_set_wrap(slice_red, 65535);
    pwm_set_wrap(slice_green, 65535);
    pwm_set_wrap(slice_blue, 65535);

    pwm_set_chan_level(slice_red, PWM_CHAN_A, 0);
    pwm_set_chan_level(slice_green, PWM_CHAN_B, 0);
    pwm_set_chan_level(slice_blue, PWM_CHAN_A, 0);

    pwm_set_enabled(slice_red, true);
    pwm_set_enabled(slice_green, true);
    pwm_set_enabled(slice_blue, true);

    // Configuração dos botões
    gpio_init(BUTTON_A_PIN);
    gpio_init(BUTTON_B_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_set_dir(BUTTON_B_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_pull_up(BUTTON_B_PIN);

    // Configuração das interrupções
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_a_irq);
    gpio_set_irq_enabled_with_callback(BUTTON_B_PIN, GPIO_IRQ_EDGE_FALL, true, &button_b_irq);

    // Inicialização da matriz de LEDs WS2812
    ws2812_init(WS2812_PIN, NUM_LEDS);

    // Loop principal
    while (true) {
        blink_red_led();
        display_number(current_number);
        sleep_ms(100);
    }
}

// Função para piscar o LED vermelho
void blink_red_led() {
    pwm_set_gpio_level(LED_RGB_RED_PIN, 65535); // Liga o LED vermelho
    sleep_ms(100);
    pwm_set_gpio_level(LED_RGB_RED_PIN, 0); // Desliga o LED vermelho
    sleep_ms(100);
}

// Função para exibir números na matriz de LEDs WS2812
void display_number(int number) {
    // Padrões para cada número (0-9)
    const uint32_t patterns[10][25] = {
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1}, // 0
        {0, 0, 1, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 1, 1, 1, 0}, // 1
        {1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 0, 1, 0, 0, 0, 0, 1, 1, 1, 1, 1}, // 2
        {1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0}, // 3
        {1, 0, 0, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0}, // 4
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0}, // 5
        {1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0}, // 6
        {1, 1, 1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 0, 0}, // 7
        {1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0}, // 8
        {1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 1, 1, 1, 0, 0, 0, 0, 1, 0, 1, 1, 1, 1, 0}  // 9
    };

    if (number >= 0 && number <= 9) {
        for (int i = 0; i < NUM_LEDS; i++) {
            if (patterns[number][i]) {
                ws2812_set_pixel(i, 255, 0, 0); // Vermelho
            } else {
                ws2812_set_pixel(i, 0, 0, 0);   // Apagado
            }
        }
        ws2812_show();
    }
}

// Função de interrupção para o botão A
void button_a_irq(uint gpio, uint32_t events) {
    if (to_ms_since_boot(get_absolute_time()) - last_button_press > DEBOUNCE_TIME) {
        current_number = (current_number + 1) % 10;
        last_button_press = to_ms_since_boot(get_absolute_time());
    }
}

// Função de interrupção para o botão B
void button_b_irq(uint gpio, uint32_t events) {
    if (to_ms_since_boot(get_absolute_time()) - last_button_press > DEBOUNCE_TIME) {
        current_number = (current_number - 1 + 10) % 10;
        last_button_press = to_ms_since_boot(get_absolute_time());
    }
}