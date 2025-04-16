#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "driver/i2c_master.h"

#include "Screen.h"
#include "HALL_sensor.h"

void app_main(void){
    // i2c_master_dev_handle_t oled_dev_handle = oled_lcd_init(NULL);
    // oled_lcd_print(oled_dev_handle);
    adc_continuous_handle_t hall = NULL;
    uint8_t *result = NULL;
    init_hall(channel, sizeof(channel) / sizeof(adc_channel_t), &hall, result);
    uint32_t ret_num = 0;
    while(1){
        update_hall(hall, result, ret_num);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return;
}