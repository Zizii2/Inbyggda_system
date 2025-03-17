#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include <driver/ledc.h>
#include "Step_motor.h"
#include "Screen.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY_MAX           (8192.0)
#define LEDC_FREQUENCY          (50) // Frequency in Hertz

void app_main(void){
    int DB_arr[8] = {1,2,3,4,5,6,7,8};
    screen_build build = {
        .DB_arr = DB_arr,
        .enable_pin = GPIO_NUM_23,
        .is_8_bit_flag = true,
        // .read_write_pin = -1
        .RS_pin = GPIO_NUM_22
    };
    screen_ptr screen_handle = init_screen(&build);
    
    while(1){
        
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}