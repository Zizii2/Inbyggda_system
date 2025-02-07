#include <stdio.h>
#include "freertos/freertos.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "esp_err.h"

#include "esp_task_wdt.h"

#define intrp_pin               GPIO_NUM_15
#define btn_ipt                 GPIO_NUM_16

static void IRAM_ATTR gpio_isr_handler(void *arg){
    gpio_isr_handler_remove(intrp_pin);
    bool *button = (bool *) arg;
    *button = true;
    // gpio_isr_handler_add(intrp_pin, gpio_isr_handler, NULL);
    // gpio_intr_enable(intrp_pin);
}

static void init(void){
    gpio_config_t pin_config = {
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_POSEDGE,
        .pin_bit_mask = 1ull << intrp_pin,
        .pull_down_en = GPIO_PULLDOWN_ENABLE,
        .pull_up_en = GPIO_PULLUP_DISABLE
    };
    gpio_config(&pin_config);
    gpio_install_isr_service(0);
    // gpio_intr_enable(intrp_pin);

    // pin_config.mode = GPIO_MODE_INPUT;
    // pin_config.intr_type = GPIO_INTR_DISABLE;
    // pin_config.pin_bit_mask = btn_ipt;
    // pin_config.pull_down_en = GPIO_PULLDOWN_ENABLE;
    // pin_config.pull_up_en = GPIO_PULLUP_DISABLE;

    // esp_task_wdt_config_t task_wdt = {
    //     .timeout_ms = 3000,
    //     .idle_core_mask = (1 << CONFIG_FREERTOS_NUMBER_OF_CORES) - 1,
    //     .trigger_panic = false
    // };
    // ESP_ERROR_CHECK(esp_task_wdt_reconfigure(&task_wdt));
}


/*
    !Keybindings ctrl + t + x == stoppa terminal
*/
void app_main(void)
{
    printf("HHHEEEEEJJ\n");
    bool btn_pres = false;
    bool interrupt_is_on = true;
    init();

    gpio_isr_handler_add(intrp_pin, gpio_isr_handler, &btn_pres);

    // ESP_ERROR_CHECK(esp_task_wdt_add(NULL));
    int level = 0;
    TickType_t button_pressed_time;
    button_pressed_time = xTaskGetTickCount();
    while(true){
        level = gpio_get_level(intrp_pin);
        // printf("lvl: %d | bool: %d\n", level, &btn_pres);
        if(btn_pres){
            // ESP_ERROR_CHECK(esp_task_wdt_reset());
            printf("RESET TASK\n");
            btn_pres = false;
            button_pressed_time = xTaskGetTickCount();
            interrupt_is_on = false;
            
        }

        if(!interrupt_is_on){
            TickType_t timeElapsed = xTaskGetTickCount() - button_pressed_time;
            if (timeElapsed > pdMS_TO_TICKS(40)){
                gpio_isr_handler_add(intrp_pin, gpio_isr_handler, &btn_pres);
                interrupt_is_on = true;
            }
        }
        // ESP_ERROR_CHECK(esp_task_wdt_reset());
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}