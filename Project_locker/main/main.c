#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <driver/gpio.h>
#include "driver/i2c_master.h"
#include "Screen.h"

void app_main(void){
    i2c_master_dev_handle_t oled_dev_handle = oled_lcd_init(NULL);
    i2c_master_bus_handle_t master_handle;
    ESP_ERROR_CHECK(i2c_master_get_bus_handle(0, &master_handle));
    if(oled_dev_handle == NULL){
        ESP_LOGE("DEVICE OLED LCD", "handle not found!");
        return;
    }
    oled_lcd_print(oled_dev_handle);
    return;
}