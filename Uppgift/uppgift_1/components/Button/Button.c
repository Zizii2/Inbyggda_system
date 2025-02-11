#include <stdio.h>
#include "Button.h"

#define TIME_TO_WAIT_FOR_DEBOUNCE pdMS_TO_TICKS(30)
#define ON  (1)
#define OFF (0)

/// @brief Initializes the GPIO pins for the buttons
/// @param new_intr the choosen interrupt type
/// @param new_mode the choosen mode (mainly input or output)
/// @param new_pull_up the pullup type (ENABLE or DISABLE)
/// @param new_pull_down the pulldown type (ENABLE or DISABLE)
/// @param varadic expected data type: `gpio_num_t` this arg is for the choosen the button pins by number
/// @return `esp_err_t` ESP_OK = 0 or ESP_FAIL = -1
button_handel init(gpio_int_type_t new_intr, gpio_mode_t new_mode, gpio_pullup_t new_pull_up, gpio_pulldown_t new_pull_down, gpio_num_t new_pin){
    gpio_config_t button_config;
    button_t *btn = (button_t*)malloc(sizeof(button_t));
    if (GPIO_INTR_DISABLE <= new_intr && new_intr <= GPIO_INTR_MAX)
    { button_config.intr_type = new_intr; }
    else { return ESP_FAIL; }

    if (GPIO_MODE_DISABLE <= new_mode && new_mode <= GPIO_MODE_INPUT_OUTPUT)
    { button_config.mode = new_mode; }
    else { return ESP_FAIL; }

    if (GPIO_PULLUP_DISABLE <= new_pull_up && new_pull_up <= GPIO_PULLUP_ENABLE)
    { button_config.pull_up_en = new_pull_up; }
    else { return ESP_FAIL; }

    if (GPIO_PULLDOWN_DISABLE <= new_pull_down && new_pull_down <= GPIO_PULLDOWN_ENABLE)
    { button_config.pull_down_en = new_pull_down; }
    else { return ESP_FAIL; }

    button_config.pin_bit_mask = 1ULL << new_pin;
    ESP_ERROR_CHECK(gpio_config(&button_config));

    // Initialize button_t struct
    btn->btn_pin = new_pin;
    btn->latch = OFF;
    return btn;
}

bool update(button_handel btn, TickType_t elapsed_time){
    btn->level = gpio_get_level(btn->btn_pin);
    if (btn->level == ON && btn->latch == OFF){        // Button goes from off to on
        if (elapsed_time >= TIME_TO_WAIT_FOR_DEBOUNCE){ 
            btn->latch = ON;
            return true;
        }
    }
    else if (btn->level == OFF && btn->latch == ON){   // Button goes from on to off
        if (elapsed_time >= TIME_TO_WAIT_FOR_DEBOUNCE){
            btn->latch = OFF;
            return true;
        }
    }
    return false;
}

bool isPressed(int latch){
    return (latch == OFF) ? false : true;
}

void setOnPressed(void (*onPressed)(int)){
    //? är det här tillåtet
    //btn->cb = onPressed;
}
