#pragma once
#include "driver/gpio.h"
// #include "driver/ledc.h"

typedef struct Buzz_config{
    gpio_num_t output_pin;
} Buzz_config;

typedef struct Buzz_t{
    gpio_num_t output_pin;
    bool state;
} Buzz_t;

typedef Buzz_t *Buzz_handle;

Buzz_handle init_buzz(Buzz_config *cfg);
void update_buzz(Buzz_handle buzz);
void make_noise(Buzz_handle buzz);
void stop_noise(Buzz_handle buzz);
