#pragma once
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ARR_RAW_MAX 100
#define ARR_RAW_START 0

typedef void(*callback_poten)(adc1_channel_t, int);

typedef struct Potentiometer_config_t{
    adc1_channel_t channel;
    gpio_num_t pin;
    adc_atten_t atten;
    adc_bits_width_t width;
    bool use_channel;
    bool use_pin;
} Potentiometer_config_t;

typedef struct Potentiometer_t{
    gpio_num_t pin;
    adc1_channel_t channel;
    adc_bits_width_t width;
    adc_atten_t atten;
    int arr_raw[ARR_RAW_MAX];
    int idx;
    int prev_sum;
    int threshold;
    bool on_rising_edge;
    callback_poten cb;

}Potentiometer_t;

typedef Potentiometer_t *Poten_handel;

Poten_handel init_poten(Potentiometer_config_t *poten_config);
void update_poten(Poten_handel poten);
int getValue_poten(Poten_handel poten, int data);
void setOnThreshold_poten(Poten_handel poten, int threshold, bool on_rising_dge, void (*onThreshold)(adc1_channel_t channel, int value));

