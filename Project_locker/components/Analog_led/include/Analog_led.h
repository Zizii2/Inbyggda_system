#pragma once
#include "math.h"
#include "esp_err.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_1
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY_MAX           (8192.0)
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

#define STEP_OF_PERIOD          (0.01)
#define ANGEL_INCREASE          (0.05)

#define pwm_step                (100)

static enum Analog_led_state_t{ CONST_STATE=0, SIN_STATE=1, PWM_ON=2, PWM_OFF=3};

typedef struct Analog_led_config{
    gpio_num_t output_pin;
    ledc_channel_t channel;
    int duty;
} Analog_led_config;

typedef struct Analog_led_t{
    gpio_num_t pin;
    enum Analog_led_state_t state;
    TickType_t last_updated;
    double period;
    double duty;
    double angel;
} Analog_led_t;

typedef Analog_led_t *Analog_led_handel;

//lägg till mer parametrar senare för init
Analog_led_handel init_analog(Analog_led_config* led_config);
void update_analog(Analog_led_handel led, TickType_t curr_time);
void set_led_analog(Analog_led_handel led, int new_value );
void sin_analog(Analog_led_handel led, int new_period);
void pwm_on_analog(Analog_led_handel led);
void pwm_off_analog(Analog_led_handel led);