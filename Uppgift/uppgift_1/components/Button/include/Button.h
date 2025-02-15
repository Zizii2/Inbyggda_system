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

typedef void(*callback_btn)(void *);

static enum Button_state{
    OFF_STATE,
    ON_STATE,
    ON_TO_OFF_STATE,
    OFF_TO_ON_STATE
};

static enum Cb_state_btn{
    NOT_INIT_BTN,
    DISABLED_BTN,
    ENABLED_BTN
};

typedef struct button_config_t{
    gpio_int_type_t intr;
    gpio_pullup_t pull_up;
    gpio_pulldown_t pull_down;
    gpio_num_t input_pin;
    bool use_debug_txt;
} button_config_t;

typedef struct button_t{
    int level;
    enum Cb_state_btn do_callback;
    enum Button_state state;
    gpio_num_t btn_pin;
    callback_btn cb;
    void *data;
    TickType_t last_updated;
    bool debug_txt;
} button_t;

typedef button_t* button_handel;

button_handel init_button(button_config_t *button_config);
void update_button(button_handel btn);
bool isPressed_button(button_handel btn);
void setOnPressed_button(button_handel btn, void (*onPressed)(void *), void *data);