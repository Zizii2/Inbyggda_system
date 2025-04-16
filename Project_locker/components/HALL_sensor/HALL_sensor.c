#include <stdio.h>
#include "HALL_sensor.h"

void init_hall(gpio_num_t pin, hall_handle *out_handle){
   gpio_config_t cfg = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_INPUT,
        .pin_bit_mask = (1ULL << pin),
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE,
   };
   gpio_config(&cfg);
   hall_handle handle = (hall_handle)malloc(sizeof(hall_t));
   handle->pin = pin;
   handle->prev_val = 1;
   *out_handle = handle;
}

void update_hall(hall_handle hall){
    int value = gpio_get_level(hall->pin);
    if(value != hall->prev_val){
        hall->prev_val = value;
    }
}

int get_hall_val(hall_handle hall){
    return hall->prev_val;
}