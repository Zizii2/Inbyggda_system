#include <stdio.h>
#include "Piezo_speaker.h"

Buzz_handle init_buzz(Buzz_config *cfg)
{
    gpio_config_t pin_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << cfg->output_pin,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
    };
    gpio_config(&pin_config);

    Buzz_handle handle  = (Buzz_handle)malloc(sizeof(Buzz_t));
    handle->output_pin  = cfg->output_pin;
    handle->state       = false;
    return handle;
}

void update_buzz(Buzz_handle buzz){
    if(buzz->state){
        gpio_set_level(buzz->output_pin, 1);
    }
    else {
        gpio_set_level(buzz->output_pin, 0);
    }
}

void make_noise(Buzz_handle buzz){
    buzz->state = true;
}

void stop_noise(Buzz_handle buzz){
    buzz->state = false;
}