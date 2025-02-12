#pragma once
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

static enum Binary_led_value_t{ LOW=0, HIGH=1 };
static enum Binary_led_state{ SET_STATE=0, BLINK_STATE=1 };

typedef struct Binary_led_t{
    gpio_num_t pin;
    enum Binary_led_state state;
    enum Binary_led_value_t value;
    TickType_t last_updated;
    TickType_t blink_on_timer;
    TickType_t blink_off_timer;
} Binary_led_t;

typedef Binary_led_t *Bin_led_handel;

Bin_led_handel init (gpio_num_t new_output_pin, enum Binary_led_value_t new_value);
void update (Bin_led_handel led, TickType_t curr_time);
void setLed (Bin_led_handel led, enum Binary_led_value_t new_value); 
void blink(Bin_led_handel led, unsigned int miliseocnds_on, unsigned int milliseconds_off);

