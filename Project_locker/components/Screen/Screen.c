#include <stdio.h>
#include <string.h>
#include "driver/gpio.h"
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
// #include "hal/ledc_types.h"
// #include "esp_lcd_panel_ops.h"
// #include "esp_lcd_io_spi.h"
// #include "esp_lcd_panel_vendor.h"
#include "Screen.h"

screen_ptr init_screen(screen_build *build)
{
    screen_ptr s = (screen_ptr)malloc(sizeof(screen_t));
    memset(s, 0, sizeof(screen_t));
    gpio_config_t DB_config = {
        .intr_type = GPIO_INTR_DISABLE,
        .mode = GPIO_MODE_OUTPUT,
        .pin_bit_mask = (1ULL << build->DB_arr[0]) 
                        || (1ULL << build->DB_arr[1])
                        || (1ULL << build->DB_arr[2])
                        || (1ULL << build->DB_arr[3])
                        || (1ULL << build->DB_arr[4])
                        || (1ULL << build->DB_arr[5])
                        || (1ULL << build->DB_arr[6])
                        || (1ULL << build->DB_arr[7]),
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&DB_config);
    //Start of init
    vTaskDelay(pdMS_TO_TICKS(20));
    gpio_set_level(build->DB_arr[5], 1);
    gpio_set_level(build->DB_arr[4], 1);
    vTaskDelay(pdMS_TO_TICKS(5));
    gpio_set_level(build->DB_arr[5], 0);
    gpio_set_level(build->DB_arr[4], 0);
    gpio_set_level(build->DB_arr[3], 1);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(build->DB_arr[4], 0);
    gpio_set_level(build->DB_arr[0], 1);
    vTaskDelay(pdMS_TO_TICKS(1));
    gpio_set_level(build->DB_arr[0], 0);
    gpio_set_level(build->DB_arr[2], 1); // det stog DB3 ska vara 1 (med det fanns två så jag gissar lite
    //end of init
    
    return s;
}
