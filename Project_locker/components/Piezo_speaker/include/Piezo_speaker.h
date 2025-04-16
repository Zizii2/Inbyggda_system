#pragma once
#include "driver/gpio.h"
#include "driver/ledc.h"

#define BUZZ_TIMER              LEDC_TIMER_0
#define BUZZ_MODE               LEDC_LOW_SPEED_MODE
#define BUZZ_CHANNEL            LEDC_CHANNEL_0
#define BUZZ_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define BUZZ_AUTO_CLK           LEDC_AUTO_CLK
#define BUZZ_INTR_DISABLE       LEDC_INTR_DISABLE
#define BUZZ_DUTY_MAX           (8192.0)
#define BUZZ_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

typedef struct Buzz_config{
    gpio_num_t output_pin;
    ledc_channel_t channel;
    int duty;
} Buzz_config;

typedef struct Buzz_t{
    gpio_num_t output_pin;
    ledc_channel_t channel;
    int duty;
    bool state;
} Buzz_t;

typedef Buzz_t *Buzz_handle;

Buzz_handle init_buzz(Buzz_config *cfg);
void make_noise(Buzz_handle buzz);
void stop_noise(Buzz_handle buzz);
