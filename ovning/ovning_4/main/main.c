#include <stdio.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#include "esp_task_wdt.h"

#define intrp_pin               GPIO_NUM_12
#define btn_ipt                 GPIO_NUM_15

bool btn_pres = false;

static void IRAM_ATTR gpio_isr_handler(void *arg){
    //TODO write code to do something

    btn_pres = true;

    gpio_isr_handler_add(intrp_pin, gpio_isr_handler, NULL);
    gpio_intr_enable(intrp_pin);
    
}

static void init(void){
    gpio_config_t pin_config = {
        .mode = GPIO_MODE_INPUT_OUTPUT_OD,
        .intr_type = GPIO_INTR_NEGEDGE,
        .pin_bit_mask = intrp_pin,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .pull_up_en = GPIO_PULLUP_ENABLE
    };
    gpio_config(&pin_config);

    pin_config.mode = GPIO_MODE_INPUT;
    pin_config.intr_type = GPIO_INTR_DISABLE;
    pin_config.pin_bit_mask = btn_ipt;
    pin_config.pull_down_en = GPIO_PULLDOWN_ENABLE;
    pin_config.pull_up_en = GPIO_PULLUP_DISABLE;

    gpio_install_isr_service(0);
    gpio_isr_handler_add(intrp_pin, gpio_isr_handler, NULL);
    gpio_intr_enable(intrp_pin);

    esp_task_wdt_config_t task_wdt = {
        .timeout_ms = 3000,
        .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) - 1,
        .trigger_panic = true
    };
    ESP_ERROR_CHECK(esp_task_wdt_reconfigure(&task_wdt));
}

void app_main(void)
{
    init();
    ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    while(true){
        // int level = gpio_get_level(intrp_pin);
        // printf("%d\n", level);
        gpio_isr_handler_add(intrp_pin, gpio_isr_handler, NULL);
        gpio_intr_enable(intrp_pin);
        if(btn_pres){
            btn_pres = false;
            ESP_ERROR_CHECK(esp_task_wdt_reset());
            printf("RESET TASK\n");
            vTaskDelay(pdMS_TO_TICKS(30));
        }
    }
}