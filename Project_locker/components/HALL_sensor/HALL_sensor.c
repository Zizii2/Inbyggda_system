#include <stdio.h>
#include "HALL_sensor.h"

void init_hall(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle, uint8_t *out_result){
    adc_continuous_handle_t hall_handle = NULL;
    adc_continuous_handle_cfg_t hall_config = {
        .max_store_buf_size = 1024,
        .conv_frame_size = 256,
    };
    ESP_ERROR_CHECK(adc_continuous_new_handle(&hall_config, &hall_handle));

    adc_continuous_config_t dig_cfg = {
        .sample_freq_hz = 20 * 1000,
        .conv_mode = EXAMPLE_ADC_CONV_MODE,
        .format = EXAMPLE_ADC_OUTPUT_TYPE,
    };

    adc_digi_pattern_config_t adc_pattern[SOC_ADC_PATT_LEN_MAX] = {0};
    dig_cfg.pattern_num = channel_num;
    for (int i = 0; i < channel_num; i++) {
        adc_pattern[i].atten = EXAMPLE_ADC_ATTEN;
        adc_pattern[i].channel = channel[i] & 0x7;
        adc_pattern[i].unit = EXAMPLE_ADC_UNIT;
        adc_pattern[i].bit_width = EXAMPLE_ADC_BIT_WIDTH;

        ESP_LOGI(TAG, "adc_pattern[%d].atten is :%"PRIx8, i, adc_pattern[i].atten);
        ESP_LOGI(TAG, "adc_pattern[%d].channel is :%"PRIx8, i, adc_pattern[i].channel);
        ESP_LOGI(TAG, "adc_pattern[%d].unit is :%"PRIx8, i, adc_pattern[i].unit);
    }
    dig_cfg.adc_pattern = adc_pattern;
    ESP_ERROR_CHECK(adc_continuous_config(hall_handle, &dig_cfg));
    *out_handle = hall_handle;

    uint8_t result[EXAMPLE_READ_LEN] = {0};
    memset(result, 0xcc, EXAMPLE_READ_LEN);
    out_result = result;

}

// void start_monitor(void){
//     adc_monitor_config_t = {
        
//     };
// }

// int get_avg(adc_continuous_handle_t hall){
//     int avg = 0;
//     for(int i=0; i<BUFF_SIZE; i++){
//         avg += hall->buffert[i];
//     }
//     return avg/BUFF_SIZE;
// }

void update_hall(adc_continuous_handle_t hall, uint8_t *result, uint32_t ret_num){
    esp_err_t ret = adc_continuous_read(hall, result, EXAMPLE_READ_LEN, &ret_num, 0);
    if (ret == ESP_OK) {
        ESP_LOGI("TASK", "ret is %x, ret_num is %"PRIu32" bytes", ret, ret_num);
        for (int i = 0; i < ret_num; i += SOC_ADC_DIGI_RESULT_BYTES) {
            adc_digi_output_data_t *p = (adc_digi_output_data_t*)&result[i];
            uint32_t chan_num = EXAMPLE_ADC_GET_CHANNEL(p);
            uint32_t data = EXAMPLE_ADC_GET_DATA(p);
            /* Check the channel number validation, the data is invalid if the channel num exceed the maximum channel */
                ESP_LOGI(TAG, "Value: %"PRIx32, chan_num, data);
        }
        /**
         * Because printing is slow, so every time you call `ulTaskNotifyTake`, it will immediately return.
         * To avoid a task watchdog timeout, add a delay here. When you replace the way you process the data,
         * usually you don't need this delay (as this task will block for a while).
         */
    }
    // int value = adc1_get_raw(hall->channel);
    // hall->buffert[hall->buff_idx++] = value;
    // printf("value: %d\n", get_avg(hall));
}