#include <stdio.h>
#include "Binary_led.h"

Bin_led_handel init_binary(gpio_num_t new_output_pin, enum Binary_led_value_t new_value){
    Bin_led_handel led = (Bin_led_handel)malloc(sizeof(Binary_led_t));
    gpio_config_t led_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = 1ULL << new_output_pin,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&led_config);
    led->pin = new_output_pin;
    led->value = new_value;
    led->blink_on_timer = 0;
    led->blink_off_timer = 0;
    led->last_updated = xTaskGetTickCount();
    led->state = SET_STATE;
    return led;
}

void update_binary(Bin_led_handel led, TickType_t curr_time){
    if (led->state == SET_STATE){ led->last_updated = xTaskGetTickCount(); }
    else if (led->state == BLINK_STATE){
        if (led->value == HIGH){
            if(curr_time - led->last_updated >= led->blink_on_timer){
                led->last_updated = xTaskGetTickCount();
                led->value = LOW;
            }
        }
        else{ //led-> value === LOW
            if(curr_time - led->last_updated >= led->blink_off_timer){
                led->last_updated = xTaskGetTickCount();
                led->value = HIGH;
            }
        }
    }
    gpio_set_level(led->pin, led->value);
}

void set_led_binary (Bin_led_handel led, enum Binary_led_value_t new_value){
    if(led->state != SET_STATE){
        led->state = SET_STATE;
        led->blink_off_timer = 0;
        led->blink_on_timer = 0;
    }
    led->value = new_value;
}

void blink_binary(Bin_led_handel led, unsigned int MS_on, unsigned int MS_off){
    if (led->state != BLINK_STATE){ led->state = BLINK_STATE; }
    led->blink_off_timer = pdMS_TO_TICKS(MS_on);
    led->blink_on_timer = pdMS_TO_TICKS(MS_off);
}
