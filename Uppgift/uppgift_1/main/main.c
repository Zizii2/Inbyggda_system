#include <stdio.h>
#include "Button.h"
#include "Potentiometer.h"
#include "Binary_led.h"
#include "Analog_led.h"

void print_num(int num){
    printf("%d, nice!\n---\n", num);
}

void app_main(void){
    button_config_t btn_config = {
        .intr = GPIO_INTR_DISABLE,
        .pull_up = GPIO_PULLUP_DISABLE,
        .pull_down = GPIO_PULLDOWN_DISABLE,
        .input_pin = GPIO_NUM_2
    };
    button_handel btn1 = init_button(&btn_config);
    setOnPressed_button(btn1, print_num);
    while(1){
        update_button(btn1, xTaskGetTickCount(), 69);
        // isPressed_button(btn1, 69);
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    free(btn1);
}
// adc1_channel_t channel = ADC1_CHANNEL_5;
// Poten_handel poten = init_poten(&channel, NULL, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12);
// while(1){
    //     update_poten(poten);
    //     printf("%d\n", getValue_poten(poten));
    //     vTaskDelay(pdMS_TO_TICKS(30));
    // }
// free(poten);
    

// Bin_led_handel led_bin = init_binary(GPIO_NUM_2, HIGH);
// blink_binary(led_bin, 100, 500);

// Analog_led_config analog_config = {
//     .channel = LEDC_CHANNEL_0,
//     .duty = 0,
//     .output_pin = GPIO_NUM_3
// };
// Analog_led_handel led_ana = init_analog(&analog_config);
// sin_analog(led_ana, 1000);