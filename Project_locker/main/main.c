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
#include "Piezo_speaker.h"

void app_main(void){
    // lv_disp_t *disp = oled_lcd_init(NULL);
    // oled_lcd_print(disp, "Hello world");
    // Buzz_config buzz_cfg = {
    //     .channel = BUZZ_CHANNEL,
    //     .duty = 0,
    //     .output_pin = GPIO_NUM_11,
    // };
    // Buzz_handle buzz = init_buzz(&buzz_cfg);

    // Analog_led_config led_config = {
    //     .channel = LEDC_CHANNEL_1,
    //     .output_pin = GPIO_NUM_12,
    //     .duty = 0,
    // };
    // Analog_led_handel led1 = init_analog(&led_config);
    HALL_handle hall = init_hall();
    while(1){
        // make_noise(buzz);

        // set_led_analog(led1, 0);
        // update_analog(led1, xTaskGetTickCount());

        update_hall(hall);
        // printf("update cycle\n");

        vTaskDelay(pdMS_TO_TICKS(50));
    //     printf("lamp change\n");
        // set_led_analog(led1, 4000);
    }

    return;
}