#include <stdio.h>
#include "Analog_led.h"

#define calc_duty(angel) (((sin(angel) + 1) / 2) * LEDC_DUTY_MAX)

double find_angel(double curr_duty);

Analog_led_handel init_analog(Analog_led_config* led_config){
    gpio_config_t pin_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << led_config->output_pin,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&pin_config);

    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ledc_timer_config(&timer);

    ledc_channel_config_t channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = led_config->channel,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = led_config->output_pin,
        .duty           = led_config->duty, // Set duty to 0%
        .hpoint         = 0
    };
    ledc_channel_config(&channel);

    Analog_led_handel led = (Analog_led_handel)malloc(sizeof(Analog_led_t));
    led->pin = led_config->output_pin;
    led->state = CONST_STATE;
    led->period = 0;
    led->last_updated = xTaskGetTickCount();
    led->angel = 0.0;
    led->duty = 0.0;
    return led;
}

void update_analog(Analog_led_handel led, TickType_t curr_time){
    if(led->state == SIN_STATE){
        if(curr_time - led->last_updated >= (led->period * STEP_OF_PERIOD)){
            led->angel += (led->period * ANGEL_INCREASE); //! NEW THING HERE (MIGHT NOT WORK)
            if(led->angel > 360){ led->angel = 0; }
            led->duty = calc_duty(led->angel); 
        }
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, led->duty);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }
    if(led->state == PWM_ON){
        if(led->last_updated - curr_time < pdMS_TO_TICKS(5000)){ return; }
        led->last_updated = xTaskGetTickCount();
        if(led->duty + pwm_step > LEDC_DUTY_MAX){ led->state = CONST_STATE; }
        led->duty += pwm_step;
        printf("duty: %.3f\n", led->duty);
        ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, led->duty);
        ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
    }
}

void set_led_analog(Analog_led_handel led, int value ){
    if (led->state != CONST_STATE){ led->state = CONST_STATE; }
    if (0 <= value && value <= LEDC_DUTY_MAX){  led->duty = value; }
    else{ ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG); }
    ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, led->duty);
    ledc_update_duty(LEDC_MODE, LEDC_CHANNEL);
}

void sin_analog(Analog_led_handel led, int new_period){
    led->state = SIN_STATE;
    led->last_updated = xTaskGetTickCount();
    led->period = new_period;
    led->duty = LEDC_DUTY_MAX-1;
    led->angel = find_angel(led->duty); // antar den inte
}

double find_angel(double curr_duty){
    double angel = ANGEL_INCREASE;
    double base_duty = calc_duty(angel);
    int plus_minus_int = 3;
    while(1){
        if(curr_duty + plus_minus_int >= base_duty && curr_duty - plus_minus_int <= base_duty){
            return angel;
        }
        else if (angel == 0){
            return 90.0;
        }
        else{
            angel += ANGEL_INCREASE;
            base_duty = calc_duty(angel);
        }
        vTaskDelay(5);
    }
}

void pwm_on_analog(Analog_led_handel led){
    printf("state change\n");
    led->state=PWM_ON;
    led->duty = 0;
    led->last_updated = xTaskGetTickCount();
}