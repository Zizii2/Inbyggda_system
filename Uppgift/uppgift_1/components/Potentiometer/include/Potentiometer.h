#pragma once
#include "driver/adc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define ARR_RAW_MAX 100
#define ARR_RAW_START 0

typedef void(*callback_poten)(adc1_channel_t, int);

static enum Cb_state_poten{
    NOT_INIT_POTEN,
    THRES_EN_POTEN,
    THRES_DIS_POTEN,
    RISE_EN_POTEN,
    RISE_DIS_POTEN
};

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
    enum Cb_state_poten do_callback;
    callback_poten cb;
    int data;
}Potentiometer_t;

typedef Potentiometer_t *Poten_handel;

Poten_handel init_poten(Potentiometer_config_t *poten_config);
void update_poten(Poten_handel poten);
int getValue_poten(Poten_handel poten);
void setOnThreshold_poten(Poten_handel poten, int threshold, bool on_rising_dge, void (*onThreshold)(adc1_channel_t channel, int value), int data);

