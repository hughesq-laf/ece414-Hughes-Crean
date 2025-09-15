#include "led_out.h"
#include "pico/stdlib.h"

static const uint32_t LED_PORT_MASK = 0b00000000000000000000001111111100;

void led_out_init() {
    gpio_init_mask(LED_PORT_MASK);
    gpio_set_dir_out_masked(LED_PORT_MASK);
}

void led_out_write(uint8_t value) {
    gpio_put_masked(LED_PORT_MASK, value << 2);
}