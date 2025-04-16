#pragma once
#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_err.h"
#include "esp_log.h"
#include "driver/i2c_master.h"
#include "esp_lvgl_port.h"
#include "lvgl.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_lcd_panel_ssd1306.h"
#include <string.h>


#ifdef TAG
#undef TAG
#endif
#define TAG "Display"

#define I2C_BUS_PORT  0

#define LCD_PIXEL_CLOCK_HZ    (400 * 1000)
#define PIN_NUM_SDA           6
#define PIN_NUM_SCL           7
#define PIN_NUM_RST           -1
#define I2C_HW_ADDR           0x3C

#define LCD_H_RES              128
#define LCD_V_RES              64

// Bit number used to represent command and parameter
#define LCD_CMD_BITS           8
#define LCD_PARAM_BITS         8

typedef struct screen_t{
    lv_disp_t * disp;
    lv_obj_t *label;
}screen_t;

typedef screen_t* screen_handle;

screen_handle oled_lcd_init(i2c_master_bus_handle_t master_handle, gpio_num_t scl_io, gpio_num_t sda_io);
void oled_lcd_print(screen_handle screen, const char* txt);
