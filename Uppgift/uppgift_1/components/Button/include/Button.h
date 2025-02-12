#pragma once
#include <stdarg.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TIME_TO_WAIT_FOR_DEBOUNCE pdMS_TO_TICKS(30)
#define OFF         (0)
#define ON          (1)

typedef void(*callback)(int);

typedef struct button_t{
    int level;
    int latch;
    gpio_num_t btn_pin;
    callback cb;
    TickType_t last_updated;
} button_t;

typedef button_t* button_handel;

button_handel init_button(gpio_int_type_t new_intr, gpio_pullup_t new_pull_up,
                        gpio_pulldown_t new_pull_down, gpio_num_t new_input_pin);
void update_button(button_handel btn, TickType_t curr_time);
bool isPressed_button(button_handel btn);
void setOnPressed_button(button_handel btn, void (*onPressed)(int)); // möjlighet av att ha en void* för "user_data"