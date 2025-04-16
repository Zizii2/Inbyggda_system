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
    HALL_handle hall = init_hall();
    while(1){
        update_hall(hall);
        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return;
}