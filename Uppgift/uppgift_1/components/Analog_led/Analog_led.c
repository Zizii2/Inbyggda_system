#include <stdio.h>
#include "Analog_led.h"

Analog_led_handel init_analog(gpio_num_t new_output_pin){
    Analog_led_handel led = (Analog_led_handel)malloc(sizeof(Analog_led_t));
    // gpio_config_t led_config = {
    //     .intr_type = GPIO_INTR_DISABLE,
    //     .mode = GPIO_MODE_OUTPUT,
    //     .pin_bit_mask = 1ULL << new_output_pin,
    //     .pull_down_en = GPIO_PULLDOWN_ENABLE,
    //     .pull_up_en = GPIO_PULLUP_DISABLE
    // };
    // gpio_config(&led_config);

    led->pin = new_output_pin;
    led->state = CON_STATE;
    led->period_step = 0;
    led->angel = 0.0;
    led->duty = 0.0;

    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer));

    ledc_channel_config_t channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = new_output_pin,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel));
    return led;
}

void update_analog(Analog_led_handel led){
    if(led->state == SIN_STATE){
        led->angel += led->period_step;
        led->duty = sin(led->angel) * LEDC_DUTY_MAX;
    }
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, led->duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void set_led_analog(Analog_led_handel led, int value ){
    if (led->state != CON_STATE){
        led->state = CON_STATE;
        led->period_step = 0;
    }
    //TODO lägg till fel hantering, i hela koden
    led->duty = value;
}

void sin_analog(Analog_led_handel led, int new_period){
    led->state = SIN_STATE;
    led->period_step = new_period/360.0;
    //! inte perfekt lösning, men funkar för nu fället
    led->duty = LEDC_DUTY_MAX-1;
    led->angel = 90.0;
}