#pragma once
#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2c_master.h"

/*
    TODO figure out if it nesseccery to send the pointer of the handle or if this works as well
*/
i2c_master_dev_handle_t oled_lcd_init(i2c_master_bus_handle_t master_handle);

void oled_lcd_print(i2c_master_dev_handle_t oled_dev_handle);
