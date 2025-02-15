#pragma once
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ARR_RAW_MAX 100
#define ARR_RAW_START 0

typedef void(*callback_poten)(adc1_channel_t, void *);

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
    int arr_raw[ARR_RAW_MAX];
    int idx;
    int prev_sum;
    int threshold;
    bool on_rising_edge;
    bool do_callback;
    callback_poten cb;
    void *data;
}Potentiometer_t;

typedef Potentiometer_t *Poten_handel;

Poten_handel init_poten(Potentiometer_config_t *poten_config);
void update_poten(Poten_handel poten);
float getValue_poten(Poten_handel poten);
void setOnThreshold_poten(Poten_handel poten, int threshold, bool on_rising_dge, void (*onThreshold)(adc1_channel_t channel, void *value), void *data);

