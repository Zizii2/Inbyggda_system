#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/ledc.h"
#include "esp_err.h"

#include "driver/gpio.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (5) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY_MAX           (8192)
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

#define INPUT_1                 (GPIO_NUM_20)
#define OUTPUT_LED              (GPIO_NUM_15)

static void init_LEDC(void){
    ledc_timer_config_t timer = {
        .speed_mode       = LEDC_MODE,
        .duty_resolution  = LEDC_DUTY_RES,
        .timer_num        = LEDC_TIMER,
        .freq_hz          = LEDC_FREQUENCY,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&timer));

    ledc_channel_config_t channel = {
        .speed_mode     = LEDC_MODE,
        .channel        = LEDC_CHANNEL,
        .timer_sel      = LEDC_TIMER,
        .intr_type      = LEDC_INTR_DISABLE,
        .gpio_num       = LEDC_OUTPUT_IO,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0
    };
    ESP_ERROR_CHECK(ledc_channel_config(&channel));

    // //TODO code here
    gpio_config_t io_conf = {};
    //disable interrupt
    io_conf.intr_type = GPIO_INTR_DISABLE;
    //set as output mode
    io_conf.mode = GPIO_MODE_OUTPUT;
    //bit mask of the pins that you want to set
    io_conf.pin_bit_mask = OUTPUT_LED;
    //disable pull-down mode
    io_conf.pull_down_en = 0;
    //disable pull-up mode
    io_conf.pull_up_en = 0;
    //configure GPIO with the given settings
    gpio_config(&io_conf);

    /*
        input_1
    */
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins
    io_conf.pin_bit_mask = INPUT_1;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

static int btn_input(int button){
    TickType_t start_tick = xTaskGetTickCount();
    int level = 0, latch = 0;
    
    level = gpio_get_level(button);
    /*
        button press or start of hold
    */
    if (level == 1 && latch == 0){
        TickType_t tick_diff = xTaskGetTickCount() - start_tick;
        if (tick_diff >= pdMS_TO_TICKS(30)){
            start_tick = xTaskGetTickCount();
            latch = 1;
        }
    }
    /*
        button lift or end of hold
    */
    else if (level == 0 && latch == 0){
        TickType_t tick_diff = xTaskGetTickCount() - start_tick;
        if (tick_diff >= pdMS_TO_TICKS(30)){
            latch = 0;
        }
    }
}

static void execute_w_btn(){
    int duty = 0;
    while(true){
        int ipt_1 = btn_input;
        printf("Btn: %d | ", ipt_1);
        if(ipt_1 == false){
            duty = 0;
            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        }
        else{
            if(duty + 100 <= LEDC_DUTY_MAX){
                duty+=100;
            }

            ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
            ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
        }
        int outpt = gpio_get_level(OUTPUT_LED);
        printf("Out: %d\n", outpt);
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

void app_main(void)
{
    init_LEDC();
    execute_w_btn();
    // gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
}