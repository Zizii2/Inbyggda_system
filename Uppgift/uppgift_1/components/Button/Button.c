#include <stdio.h>
#include "Button.h"

/// @brief Initializes the GPIO pins for the buttons
/// @param new_intr the choosen interrupt type
/// @param new_mode the choosen mode (mainly input or output)
/// @param new_pull_up the pullup type (ENABLE or DISABLE)
/// @param new_pull_down the pulldown type (ENABLE or DISABLE)
/// @param gpio_num_t the choosen pin for the button
/// @return `button_handel` or `NULL`

/*
    notes:
    gpio_mode går att göra konstant, för idéen är väl att pinnen ska bara kunna ta emot data från knappen

    med hur Potensiometer del går det kan är bara att ha något mer liknande gpio_config_t men bara minska valet så det är mer "streamlined"
    DVS. göra min egna button_config_t som har mindre alternativ och är gjort bara för knappar
*/
button_handel init_button(gpio_int_type_t new_intr, gpio_pullup_t new_pull_up, gpio_pulldown_t new_pull_down, gpio_num_t new_input_pin){
    gpio_config_t button_config;
    button_handel btn = (button_handel)malloc(sizeof(button_t));
    if (GPIO_INTR_DISABLE <= new_intr && new_intr <= GPIO_INTR_MAX)
    { button_config.intr_type = new_intr; }
    else { return NULL; }

    button_config.mode = GPIO_MODE_INPUT;

    if (GPIO_PULLUP_DISABLE <= new_pull_up && new_pull_up <= GPIO_PULLUP_ENABLE)
    { button_config.pull_up_en = new_pull_up; }
    else { return NULL; }

    if (GPIO_PULLDOWN_DISABLE <= new_pull_down && new_pull_down <= GPIO_PULLDOWN_ENABLE)
    { button_config.pull_down_en = new_pull_down; }
    else { return NULL; }

    button_config.pin_bit_mask = 1ULL << new_input_pin;
    ESP_ERROR_CHECK(gpio_config(&button_config));

    // Initialize button_t struct
    btn->btn_pin = new_input_pin;
    btn->level = 0;
    btn->latch = 0;
    return btn;
}

void update_button(button_handel btn, TickType_t curr_time){
    int new_level = gpio_get_level(btn->btn_pin);
    if (new_level ==  btn->level){ return; }
    else{
        btn->last_updated = xTaskGetTickCount();
        btn->level = new_level;
    }

    if (btn->level == ON && btn->latch == OFF){        // Button goes from off to on
        if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){ 
            btn->latch = ON;
        }
    } else if (btn->level == OFF && btn->latch == ON){   // Button goes from on to off
        if (btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){
            btn->latch = ON;
        }
    }
}

bool isPressed_button(button_handel btn){
    return (btn->latch == ON) ? true : false;
    return false;
}

void setOnPressed_button(button_handel btn, void (*onPressed)(int)){
    btn->cb = onPressed;
}
