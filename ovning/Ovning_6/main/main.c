#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_sleep.h"
#include "esp_err.h"

#define one_sec (uint64_t)1000000

void app_main(void)
{
    unsigned int count = 0;
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(one_sec));
    while(1){
        esp_light_sleep_start();
        printf("%d\n", ++count);
    }
}