#pragma once
#include <stdarg.h>
#include <stdint.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define TIME_TO_WAIT_FOR_DEBOUNCE   pdMS_TO_TICKS(3000)
#define OFF                         (0)
#define ON                          (1)

typedef void(*callback_btn)(int);

static enum Button_state{
    OFF_STATE,
    ON_STATE,
    ON_TO_OFF_STATE,
    OFF_TO_ON_STATE
};

typedef struct button_config_t{
    gpio_int_type_t intr;
    gpio_pullup_t pull_up;
    gpio_pulldown_t pull_down;
    gpio_num_t input_pin;
} button_config_t;

typedef struct button_t{
    int level;
    bool on_press;
    enum Button_state state;
    gpio_num_t btn_pin;
    callback_btn cb;
    TickType_t last_updated;
} button_t;

typedef button_t* button_handel;

button_handel init_button(button_config_t *button_config);
void update_button(button_handel btn, TickType_t curr_time, int data);
bool isPressed_button(button_handel btn);
void setOnPressed_button(button_handel btn, void (*onPressed)(int)); // möjlighet av att ha en void* för "user_data"