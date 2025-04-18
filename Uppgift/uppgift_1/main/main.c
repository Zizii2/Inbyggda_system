#include <stdio.h>
#include "Button.h"
#include "Potentiometer.h"
#include "Binary_led.h"
#include "Analog_led.h"

void print_num(void *data){
    int *num = (int *)data;
    printf("%d, nice!\n---\n", *num);
}

void print_num2(adc1_channel_t channel, int data){
    printf("%d, nice!\n---\n", data);
}

void app_main(void){
    Analog_led_config led_config = {
        .channel = LEDC_CHANNEL_0,
        .duty = 8000,
        .output_pin = GPIO_NUM_0
    };
    Analog_led_handel led = init_analog(&led_config);
    // set_led_analog(led, 4000);
    sin_analog(led, 10000.0);
    while(1){
        update_analog(led, xTaskGetTickCount());
        vTaskDelay(pdMS_TO_TICKS(10));
    }
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