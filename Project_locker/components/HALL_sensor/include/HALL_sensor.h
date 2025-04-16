#pragma once
#include "driver/gpio.h"

typedef struct hall_t{
    gpio_num_t pin;
    int prev_val;
}hall_t;

typedef hall_t *hall_handle;

void init_hall(gpio_num_t pin, hall_handle *out_handle);

void update_hall(hall_handle hall);
