#pragma once
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ARR_RAW_MAX 100

typedef struct Potentiometer_t{
    gpio_num_t pin;
    adc1_channel_t channel;
    adc_bits_width_t width;
    adc_atten_t atten;
    int arr_raw[ARR_RAW_MAX];
    int idx;

}Potentiometer_t;

typedef Potentiometer_t *Poten_handel;

Poten_handel init_poten(adc1_channel_t *new_channel, gpio_num_t *new_pin, adc_atten_t new_atten, adc_bits_width_t new_width ); //pin/adc, xxx );

void update_poten(Poten_handel poten);

int getValue_poten(Poten_handel poten);

//!Not done
void setOnThreshold ( int threshold, bool risingEdge);//, void (*onThreshold)(int pin/adc, value) );

