#pragma once
#include "math.h"
#include "driver/gpio.h"
#include "driver/ledc.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY_MAX           (8192.0)
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

static enum Analog_led_state_t{ CON_STATE=0, SIN_STATE=1 };

typedef struct Analog_led_t{
    gpio_num_t pin;
    enum Analog_led_state_t state;
    double period_step;
    double duty;
    double angel;
} Analog_led_t;

typedef Analog_led_t *Analog_led_handel;

//lägg till mer parametrar senare för init
Analog_led_handel init_analog(gpio_num_t new_output_pin);
void update_analog(Analog_led_handel led);
void set_led_analog(Analog_led_handel led, int new_value );
void sin_analog(Analog_led_handel led, int new_period);