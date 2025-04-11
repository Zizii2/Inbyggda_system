#include <stdio.h>
#include <driver/gpio.h>
#include "Step_motor.h"
#include <driver/ledc.h>

#define HIGH 8000
#define LOW 0

void turn_cw(Motor_t *motor){
    motor->state = ONE;
    motor->turnCW = true;
}

void update_motor(Motor_t *motor){
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 410);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 600);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
    ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, 819);
    ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);

    // printf("State: %d\n", motor->state);
    // switch (motor->state)
    // {
    // case ONE:
    //     gpio_set_level(motor->pin1, HIGH);
    //     gpio_set_level(motor->pin2, HIGH);
    //     gpio_set_level(motor->pin3, LOW);
    //     gpio_set_level(motor->pin4, LOW);

    //     motor->state = TWO;
    //     break;
    // case TWO:
    //     gpio_set_level(motor->pin1, LOW);
    //     gpio_set_level(motor->pin2, HIGH);
    //     gpio_set_level(motor->pin3, HIGH);
    //     gpio_set_level(motor->pin4, LOW);

    //     motor->state = THREE;
    //     break;
    // case THREE:
    //     gpio_set_level(motor->pin1, LOW);
    //     gpio_set_level(motor->pin2, LOW);
    //     gpio_set_level(motor->pin3, HIGH);
    //     gpio_set_level(motor->pin4, HIGH);

    //     motor->state = FOUR;
    //     break;
    // case FOUR:
    //     gpio_set_level(motor->pin1, HIGH);
    //     gpio_set_level(motor->pin2, LOW);
    //     gpio_set_level(motor->pin3, LOW);
    //     gpio_set_level(motor->pin4, HIGH);

    //     motor->state = ONE;
    //     break;
    // default:
    //     break;
    // }
}

//!main

// gpio_config_t pin_config = {
    //     .intr_type = GPIO_INTR_DISABLE,
    //     .mode = GPIO_MODE_OUTPUT,
    //     .pin_bit_mask = 1ULL << GPIO_NUM_7,
    //     .pull_down_en = GPIO_PULLDOWN_ENABLE,
    //     .pull_up_en = GPIO_PULLUP_DISABLE
    // };
    // gpio_config(&pin_config);
    
    // ledc_timer_config_t timer = {
    //     .speed_mode       = LEDC_MODE,
    //     .duty_resolution  = LEDC_DUTY_RES,
    //     .timer_num        = LEDC_TIMER,
    //     .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
    //     .clk_cfg          = LEDC_AUTO_CLK
    // };
    // ledc_timer_config(&timer);

    // ledc_channel_config_t channel = {
    //     .speed_mode     = LEDC_MODE,
    //     .channel        = LEDC_CHANNEL_0,
    //     .timer_sel      = LEDC_TIMER,
    //     .intr_type      = LEDC_INTR_DISABLE,
    //     .gpio_num       = GPIO_NUM_6,
    //     .duty           = 0,
    //     .hpoint         = 0
    // };
    // ledc_channel_config(&channel);

    // //Extra för min 12v motor
    // //TODO PWM med två kannaler (en för A+/A- och en för B+/B-)
    // //!komihåg att försjkuta en av kannalerna (klockviss ska B kannalen vara försjkutet fram)

    // Motor_t motor = {
    //     .pin1 = BLACK,
    //     .pin2 = GREEN,
    //     .pin3 = BLUE,
    //     .pin4 = RED,
    //     .turnCW = false
    // };
