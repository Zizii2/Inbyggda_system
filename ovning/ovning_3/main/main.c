#include <stdio.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include "driver/ledc.h"
#include "esp_err.h"

#include "esp_adc/adc_oneshot.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "driver/gpio.h"

#define LEDC_TIMER              LEDC_TIMER_0
#define LEDC_MODE               LEDC_LOW_SPEED_MODE
#define LEDC_OUTPUT_IO          (5) // Define the output GPIO
#define LEDC_CHANNEL            LEDC_CHANNEL_0
#define LEDC_DUTY_RES           LEDC_TIMER_13_BIT // Set duty resolution to 13 bits
#define LEDC_DUTY_MAX           (8192)
#define LEDC_FREQUENCY          (4000) // Frequency in Hertz. Set frequency at 4 kHz

#define INPUT_1                 (GPIO_NUM_20)
#define INPUT_2                 (GPIO_NUM_22)
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

    /*
        output to LED
    */
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

    /*
        input_2
    */
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins
    io_conf.pin_bit_mask = INPUT_2;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
}

static void execute_w_btn(){
    int duty = 0, duty_inc = 500;
    TickType_t start_tick = xTaskGetTickCount();
    int ipt_1 = 0, ipt_2 = 0, latch = 0, level = 0;
    while(true){
        ipt_1 = gpio_get_level(INPUT_1);
        ipt_2 = gpio_get_level(INPUT_2);
        if(ipt_1 + ipt_2 > 1){
            vTaskDelay(pdMS_TO_TICKS(20));
            continue;
        }
        else if(ipt_1){ level = ipt_1; }
        else{ level = ipt_2; }
        // printf("btn1: %d | btn2 %d | Lch: %d | duty: %d\n", ipt_1, ipt_2, latch, duty);
        /* button press or start of hold */
        if (level == 1 && latch == 0){
            TickType_t tick_diff = xTaskGetTickCount() - start_tick;
            if (tick_diff >= pdMS_TO_TICKS(30)){
                start_tick = xTaskGetTickCount();
                latch = 1;
                // code for when button pressed
                if(ipt_1){
                    if(duty+duty_inc <= LEDC_DUTY_MAX){ duty+=duty_inc; }
                    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
                    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
                }
                else if(ipt_2){
                    if(duty-duty_inc>=0){ duty-=duty_inc; }
                    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
                    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
                }
            }
        }
        /* button lift or end of hold */
        else if (level == 0 && latch == 1){
            TickType_t tick_diff = xTaskGetTickCount() - start_tick;
            if (tick_diff >= pdMS_TO_TICKS(30)){
                latch = 0;
            }
        }
        // printf("Lvl: %d | Lch: %d", level, latch);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void init_bonus(void){
    adc_oneshot_unit_handle_t adc1_handle;
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));

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

void execute_bonus(void){
    while(true){
        //TODO fix this code so it uses ADC_ONESHOT_SOMETHING()
        int ipt = gpio_get_level(INPUT_1);
        int level = adc_oneshot_read(ADC_UNIT_1, ADC_CHANNEL_4, &ipt);
        printf("lvl: %d\n", level);
        gpio_set_level(OUTPUT_LED, level);
        vTaskDelay(pdMS_TO_TICKS(20));
    }
}

void app_main(void)
{
    init_bonus();
    execute_bonus();
    // gpio_dump_io_configuration(stdout, SOC_GPIO_VALID_GPIO_MASK);
}