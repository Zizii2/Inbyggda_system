#include "Screen.h"

lv_disp_t *oled_lcd_init(i2c_master_bus_handle_t master_handle){
    if(master_handle == NULL){
        i2c_master_bus_config_t master_config = {
            .i2c_port = 0, // Use `i2c_master_get_bus_handle()` when you need the master_bus_handle in another source file
            .scl_io_num = GPIO_NUM_12,
            .sda_io_num = GPIO_NUM_11,
            .clk_source = I2C_CLK_SRC_XTAL,
            .glitch_ignore_cnt = 7,
            .intr_priority = 1,
            // .trans_queue_depth (only in asynchronous transaction)
            //! .allow_pd = true (this be set for light sleep integration, but not suppored on this chip)
        };
        i2c_new_master_bus(&master_config, &master_handle);
    }
    else{
        ESP_ERROR_CHECK(i2c_master_get_bus_handle(0, &master_handle));
    }

    // i2c_device_config_t oled_dev_config = {
    //     .dev_addr_length = I2C_ADDR_BIT_LEN_7,
    //     .device_address = 0x28,
    //     .scl_speed_hz = 200000,      // esp32 h2 max clk speed is 400khz
    //     .scl_wait_us = 0,
    // };
    // i2c_master_dev_handle_t oled_dev_handle;
    // i2c_master_bus_add_device(master_handle, &oled_dev_config, &oled_dev_handle);
    
//!----------------------------------------------------------------------------------------------
//!----------------------------------------------------------------------------------------------
//!----------------------------------------------------------------------------------------------


    ESP_LOGI(TAG, "Install panel IO");
    esp_lcd_panel_io_handle_t io_handle = NULL;
    esp_lcd_panel_io_i2c_config_t io_config = {
        .dev_addr = I2C_HW_ADDR,
        .scl_speed_hz = LCD_PIXEL_CLOCK_HZ,
        .control_phase_bytes = 1,               
        .lcd_cmd_bits = LCD_CMD_BITS,   
        .lcd_param_bits = LCD_CMD_BITS, 

        .dc_bit_offset = 6,                     

    };
    ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(master_handle, &io_config, &io_handle));

    ESP_LOGI(TAG, "Install SSD1306 panel driver");
    esp_lcd_panel_handle_t panel_handle = NULL;
    esp_lcd_panel_dev_config_t panel_config = {
        .bits_per_pixel = 1,
        .reset_gpio_num = PIN_NUM_RST,
    };

    esp_lcd_panel_ssd1306_config_t ssd1306_config = {
        .height = LCD_V_RES,
    };
    panel_config.vendor_config = &ssd1306_config;
    ESP_ERROR_CHECK(esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));


    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

    ESP_LOGI(TAG, "Initialize LVGL");
    const lvgl_port_cfg_t lvgl_cfg = ESP_LVGL_PORT_INIT_CONFIG();
    lvgl_port_init(&lvgl_cfg);

    const lvgl_port_display_cfg_t disp_cfg = {
        .io_handle = io_handle,
        .panel_handle = panel_handle,
        .buffer_size = LCD_H_RES * LCD_V_RES,
        .double_buffer = true,
        .hres = LCD_H_RES,
        .vres = LCD_V_RES,
        .monochrome = true,
        .rotation = {
            .swap_xy = false,
            .mirror_x = false,
            .mirror_y = false,
        }
    };
    lv_disp_t *disp = lvgl_port_add_disp(&disp_cfg);
    lv_disp_set_rotation(disp, LV_DISP_ROT_NONE);
    // display_s *newDisp =pvPortMalloc(sizeof(display_s));
    // newDisp->disp = disp;

    return disp;
}

void oled_lcd_print(lv_disp_t *disp, const char* txt){
    ESP_LOGI(TAG, "Display LVGL Scroll Text");
    lv_obj_t *scr = lv_disp_get_scr_act(disp);
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR); /* Circular scroll */
    lv_label_set_text(label, txt);
    /* Size of the screen (if you use rotation 90 or 270, please use lv_display_get_vertical_resolution) */
    lv_obj_set_width(label, 128);
    lv_obj_align(label, LV_ALIGN_TOP_MID, 0, 0);
}