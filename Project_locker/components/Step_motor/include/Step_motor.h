#pragma once
typedef struct pins_t{
    gpio_num_t pin1;
    gpio_num_t pin2;
    gpio_num_t pin3;
    gpio_num_t pin4;
} pins_t;

void turn_cw(pins_t *pins);
