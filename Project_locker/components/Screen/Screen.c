#include "Screen.h"

i2c_master_dev_handle_t oled_lcd_init(i2c_master_bus_handle_t master_handle){
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

    i2c_device_config_t oled_dev_config = {
        .dev_addr_length = I2C_ADDR_BIT_LEN_7,
        .device_address = 0x28,
        .scl_speed_hz = 200000,      // esp32 h2 max clk speed is 400khz
        .scl_wait_us = 0,
    };
    i2c_master_dev_handle_t oled_dev_handle;
    i2c_master_bus_add_device(master_handle, &oled_dev_config, &oled_dev_handle);
    ESP_LOGI("INIT", "NO problems with the init");
    return oled_dev_handle;
    /*
        ! Antekningar för mig
        ! Nu har jag en master_bus_handel och en device_handle
        ! Use `i2c_master_get_bus_handle()` when you need the master_bus_handle in another source file
    */
}

void oled_lcd_print(i2c_master_dev_handle_t oled_dev_handle){
    /*
    ! read https://docs.espressif.com/projects/esp-idf/en/stable/esp32h2/api-reference/peripherals/lcd/i2c_lcd.html
    */
    uint8_t data_wr = 0x10;
    i2c_master_transmit(oled_dev_handle, &data_wr, 100, -1);
}