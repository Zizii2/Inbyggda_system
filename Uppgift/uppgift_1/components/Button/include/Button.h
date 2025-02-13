#pragma once
#include <stdarg.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TIME_TO_WAIT_FOR_DEBOUNCE   pdMS_TO_TICKS(30)
#define OFF                         (0)
#define ON                          (1)

typedef void(*callback_btn)(int);

typedef struct button_config_t{
    gpio_int_type_t intr;
    gpio_pullup_t pull_up;
    gpio_pulldown_t pull_down;
    gpio_num_t input_pin;
} button_config_t;

typedef struct button_t{
    int level;
    int latch;
    gpio_num_t btn_pin;
    callback_btn cb;
    TickType_t last_updated;
} button_t;

typedef button_t* button_handel;

button_handel init_button(button_config_t *button_config);
void update_button(button_handel btn, TickType_t curr_time);
bool isPressed_button(button_handel btn, int data);
void setOnPressed_button(button_handel btn, void (*onPressed)(int)); // möjlighet av att ha en void* för "user_data"