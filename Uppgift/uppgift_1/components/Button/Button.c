#include <stdio.h>
#include "Button.h"

/// @brief Initializes the GPIO pins for the buttons
/// @param new_intr the choosen interrupt type
/// @param new_mode the choosen mode (mainly input or output)
/// @param new_pull_up the pullup type (ENABLE or DISABLE)
/// @param new_pull_down the pulldown type (ENABLE or DISABLE)
/// @param gpio_num_t the choosen pin for the button
/// @return `button_handel` or `NULL`
button_handel init_button(button_config_t *button_config){
    // Configure GPIO pin
    gpio_config_t pin_config;
    if (GPIO_INTR_DISABLE <= button_config->intr && button_config->intr <= GPIO_INTR_MAX)
    { pin_config.intr_type = button_config->intr; }
    else { return NULL; }
    pin_config.mode = GPIO_MODE_INPUT;
    if (GPIO_PULLUP_DISABLE <= button_config->pull_up && button_config->pull_up <= GPIO_PULLUP_ENABLE)
    { pin_config.pull_up_en = button_config->pull_up; }
    else { return NULL; }
    if (GPIO_PULLDOWN_DISABLE <= button_config->pull_down && button_config->pull_down <= GPIO_PULLDOWN_ENABLE)
    { pin_config.pull_down_en = button_config->pull_down; }
    else { return NULL; }
    pin_config.pin_bit_mask = 1ULL << button_config->input_pin;
    ESP_ERROR_CHECK(gpio_config(&pin_config));
    
    // Initialize button_t struct
    button_handel btn = (button_handel)malloc(sizeof(button_t));
    btn->btn_pin = button_config->input_pin;
    btn->level = 0;
    btn->cb = NULL;
    btn->state = OFF_STATE;
    return btn;
}

void update_button(button_handel btn, TickType_t curr_time, int data){
    int new_level = gpio_get_level(btn->btn_pin);
    // printf("%d\n", btn->state);
    switch(btn->state){
        case ON_STATE:
            if(new_level != btn->level){
                btn->state = ON_TO_OFF_STATE;
                btn->last_updated = xTaskGetTickCount();
            }
            if(btn->on_press){
                btn->on_press = false;
                btn->cb(data);
            }
            break;
        case OFF_STATE:
            if(new_level != btn->level){
                btn->state = OFF_TO_ON_STATE;
                btn->on_press = true;
                btn->last_updated = xTaskGetTickCount();
            }
            break;
        case ON_TO_OFF_STATE:
            if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){ 
                btn->state = OFF_STATE;
            }
            break;
        case OFF_TO_ON_STATE:
            if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){ 
                btn->state = ON_STATE;
            }
            break;
        default:
            break;
    }
    btn->level = new_level;
}
/*  
    int new_level = gpio_get_level(btn->btn_pin);
    if (new_level != btn->level && btn->press_start){
        btn->press_start = false;
        btn->last_updated = xTaskGetTickCount();
        btn->level = new_level;
    }
    // printf("Curr_time: %d & updated: %d & elapsed: %d\n", (int)curr_time, (int)btn->last_updated, (int)(curr_time - btn->last_updated));
    if (btn->level == ON && btn->latch == OFF){        // Button goes from off to on
        if(btn->on_press){
            btn->on_press = false;
            btn->cb(data);
        }
        if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){ 
            btn->latch = ON;
        }
    } else if (btn->level == OFF && btn->latch == ON){   // Button goes from on to off
        if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){
            btn->latch = OFF;
            btn->on_press = true;
            btn->press_start = true;
        }
    }
*/

bool isPressed_button(button_handel btn){
    return (btn->state == ON_STATE) ? true : false;
}

void setOnPressed_button(button_handel btn, void (*onPressed)(int)){
    btn->cb = onPressed;
}
