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
    screen_handle screen = oled_lcd_init(NULL, GPIO_NUM_12, GPIO_NUM_11);
    oled_lcd_print(screen, "welcome");

    Buzz_config buzz_cfg = {
        .output_pin = GPIO_NUM_26,
    };
    Buzz_handle buzz = init_buzz(&buzz_cfg);
    stop_noise(buzz);

    Analog_led_config led_config = {
        .channel = LEDC_CHANNEL_1,
        .output_pin = GPIO_NUM_22,
        .duty = 0,
    };
    Analog_led_handel led1 = init_analog(&led_config);

    hall_handle hall = NULL;
    init_hall(GPIO_NUM_27, &hall);

    bool door = false;
    TickType_t last_update = xTaskGetTickCount();
    TickType_t curr_time = xTaskGetTickCount();
    while(1){
        // printf("door: %d\n", get_hall_val(hall));
        curr_time = xTaskGetTickCount();
        if(!get_hall_val(hall) && curr_time - last_update > pdMS_TO_TICKS(100) && !door){
            door = true;
            last_update = xTaskGetTickCount();
            pwm_on_analog(led1);
            oled_lcd_print(screen, "Open");
            make_noise(buzz);
        }
        else if(get_hall_val(hall) && curr_time - last_update > pdMS_TO_TICKS(100) && door){
            door = false;
            last_update = xTaskGetTickCount();
            pwm_off_analog(led1);
            oled_lcd_print(screen, "Closed");
            stop_noise(buzz);
        }
        update_analog(led1, xTaskGetTickCount());
        update_hall(hall);
        update_buzz(buzz);

        vTaskDelay(pdMS_TO_TICKS(50));
    }

    return;
}