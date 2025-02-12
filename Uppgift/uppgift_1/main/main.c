#include <stdio.h>
#include "Button.h"
#include "Potentiometer.h"

void app_main(void)
{
    button_handel btn1 = init_button(GPIO_INTR_DISABLE, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_NUM_5);
    // adc1_channel_t channel = ADC1_CHANNEL_5;
    // Poten_handel poten = init_poten(&channel, NULL, ADC_ATTEN_DB_12, ADC_WIDTH_BIT_12);
    // while(1){
        //     update_poten(poten);
        //     printf("%d\n", getValue_poten(poten));
        //     vTaskDelay(pdMS_TO_TICKS(30));
        // }
    while(1){
        if ( update_button(btn1,xTaskGetTickCount()) ){
            printf("Update true\n");
        }
        if (isPressed_button(btn1)){
            printf("Button pressed\n");
        }
        // printf("...\n");
        vTaskDelay(pdMS_TO_TICKS(30));
    }
    free(btn1);
    // free(poten);
}