#include <stdio.h>
#include "Piezo_speaker.h"

Buzz_handle init_buzz(Buzz_config *cfg)
{
    gpio_config_t pin_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << cfg->output_pin,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&pin_config);

    ledc_timer_config_t timer = {
        .speed_mode       = BUZZ_MODE,
        .duty_resolution  = BUZZ_DUTY_RES,
        .timer_num        = BUZZ_TIMER,
        .freq_hz          = BUZZ_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = BUZZ_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .speed_mode     = BUZZ_MODE,
        .channel        = cfg->channel,
        .timer_sel      = BUZZ_TIMER,
        .intr_type      = BUZZ_INTR_DISABLE,
        .gpio_num       = cfg->output_pin,
        .duty           = cfg->duty, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&channel);
    Buzz_handle handle  = (Buzz_handle)malloc(sizeof(Buzz_t));
    handle->channel     = cfg->channel;
    handle->duty        = cfg->duty;
    handle->output_pin  = cfg->output_pin;
    handle->state       = false;
    return handle;
}

void update_buzz(Buzz_handle buzz){
    if(buzz->state){ buzz->duty = 100; }
    else {buzz->duty = 0; }
    ledc_set_duty(BUZZ_MODE, BUZZ_CHANNEL, buzz->duty);
    ledc_update_duty(BUZZ_MODE, BUZZ_CHANNEL);
}

void make_noise(Buzz_handle buzz){
    buzz->state = true;
}

void stop_noise(Buzz_handle buzz){
    buzz->state = false;
}