#pragma once
#include "driver/gpio.h"
#include "esp_adc/adc_continuous.h"
#include "esp_err.h"
#include "esp_log.h"
#include "inttypes.h"

static const char *TAG = "HALL";

#define EXAMPLE_ADC_UNIT                    ADC_UNIT_1
#define _EXAMPLE_ADC_UNIT_STR(unit)         #unit
#define EXAMPLE_ADC_UNIT_STR(unit)          _EXAMPLE_ADC_UNIT_STR(unit)
#define EXAMPLE_ADC_CONV_MODE               ADC_CONV_SINGLE_UNIT_1
#define EXAMPLE_ADC_ATTEN                   ADC_ATTEN_DB_0
#define EXAMPLE_ADC_BIT_WIDTH               SOC_ADC_DIGI_MAX_BITWIDTH

#define EXAMPLE_ADC_OUTPUT_TYPE             ADC_DIGI_OUTPUT_FORMAT_TYPE1
#define EXAMPLE_ADC_GET_CHANNEL(p_data)     ((p_data)->type1.channel)
#define EXAMPLE_ADC_GET_DATA(p_data)        ((p_data)->type1.data)

#define EXAMPLE_READ_LEN                    256

static adc_channel_t channel[2] = {ADC_CHANNEL_6, ADC_CHANNEL_7};

void init_hall(adc_channel_t *channel, uint8_t channel_num, adc_continuous_handle_t *out_handle, uint8_t *out_result);

void update_hall(adc_continuous_handle_t hall, uint8_t *result, uint32_t ret_num);
