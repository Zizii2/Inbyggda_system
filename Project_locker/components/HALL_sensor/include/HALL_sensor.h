#pragma once
#include "driver/gpio.h"
#include "driver/adc.h"

#define BUFF_SIZE 100

typedef struct HALL_t{
    gpio_num_t src_pin;
    adc_channel_t channel;
    int buffert[BUFF_SIZE];
    int buff_idx;
}HALL_t;

typedef HALL_t *HALL_handle;

HALL_handle init_hall(void);

void update_hall(HALL_handle hall);
