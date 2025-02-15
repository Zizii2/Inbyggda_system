#include <stdio.h>
#include "Button.h"

/// @brief Initializes the GPIO pins for the buttons
/// @param button_config Configure struct for the button
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
    btn->do_callback = NOT_INIT_BTN;
    if(button_config->use_debug_txt){ btn->debug_txt = true; }
    else{ btn->debug_txt = false; }
    return btn;
}

void update_button(button_handel btn){
    TickType_t curr_time = xTaskGetTickCount();
    int new_level = gpio_get_level(btn->btn_pin);
    switch(btn->state){
        case ON_STATE:
            if(btn->level == 0){
                btn->state = ON_TO_OFF_STATE;
                btn->last_updated = xTaskGetTickCount();
            }
            break;
        case OFF_STATE:
            if(btn->level == 1){
                btn->state = OFF_TO_ON_STATE;
                if(btn->do_callback){ btn->do_callback = ENABLED_BTN; }
                btn->last_updated = xTaskGetTickCount();
            }
            break;
        case ON_TO_OFF_STATE:
            if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){
                if(btn->debug_txt){ printf("Off state\n"); }
                btn->state = OFF_STATE;
            }
            break;
        case OFF_TO_ON_STATE:
            if (curr_time - btn->last_updated >= TIME_TO_WAIT_FOR_DEBOUNCE){ 
                if(btn->debug_txt){ printf("On state\n"); }
                btn->state = ON_STATE;
            }
            if(btn->do_callback){
                btn->do_callback = false;
                btn->cb(btn->data);
            }
            break;
        default:
            break;
    }
    btn->level = new_level;
}

bool isPressed_button(button_handel btn){
    return (btn->state == ON_STATE) ? true : false;
}

void setOnPressed_button(button_handel btn, void (*onPressed)(void *), void *data){
    btn->cb = onPressed;
    btn->data = data;
    btn->do_callback = ENABLED_BTN;
}
