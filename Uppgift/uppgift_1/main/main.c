#include <stdio.h>
#include "Button.h"
#include "Potentiometer.h"
#include "Binary_led.h"
#include "Analog_led.h"

void print_num(void *data){
    int *num = (int *)data;
    printf("%d, nice!\n---\n", *num);
}

void print_num2(adc1_channel_t channel, void *data){
    int *num = (int *)data;
    printf("%d, nice!\n---\n", *num);
}

void app_main(void){
    Potentiometer_config_t poten_config = {
        .atten = ADC_ATTEN_DB_12,
        .channel = ADC1_CHANNEL_2,
        .pin = -1,
        .use_channel = true,
        .use_pin = false,
        .width = ADC_WIDTH_BIT_12
    };
    Poten_handel poten = init_poten(&poten_config);
    setOnThreshold_poten(poten, 2500, false, print_num2, (void *)10);
    while(1){
        update_poten(poten);
        printf("%.2f\n", getValue_poten(poten));
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    free(poten);
 
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