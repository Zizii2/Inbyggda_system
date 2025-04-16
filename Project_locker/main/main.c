#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "driver/i2c_master.h"

#include "Analog_led.h"
#include "Screen.h"
#include "HALL_sensor.h"

void app_main(void){
    lv_disp_t *disp = oled_lcd_init(NULL);
    oled_lcd_print(disp, "Hello world");
    // Analog_led_config led_config = {
    //     .channel = LEDC_CHANNEL_1,
    //     .output_pin = GPIO_NUM_12,
    //     .duty = 0,
    // };
    // Analog_led_handel led1 = init_analog(&led_config);
    // HALL_handle hall = init_hall();
    while(1){
        // set_led_analog(led1, 0);
        // update_hall(hall);
        // update_analog(led1, xTaskGetTickCount());
        // printf("update cycle\n");

        vTaskDelay(pdMS_TO_TICKS(50));
    //     printf("lamp change\n");
        // set_led_analog(led1, 4000);
    }

    return;
}