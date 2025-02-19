#include <stdio.h>
#include "esp_sleep.h"
#include "esp_err.h"

#define one_sec (uint64_t)1000

void app_main(void)
{
    int count = 0;
    ESP_ERROR_CHECK(esp_sleep_enable_timer_wakeup(one_sec));
    while(1){
        ESP_ERROR_CHECK(esp_light_sleep_start());
        printf("%d\n", ++count);
    }
}