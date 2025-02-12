#include <stdio.h>
#include "Potentiometer.h"

static bool arr_raw_init = true;

/// @brief 
/// @param new_channel Set to NULL only if `new_pin` is set
/// @param new_pin Set to NULL only if `new_channel` is set
/// @param new_atten set decibel filter on input
/// @param new_width set widht of channel bit
/// @return 
Poten_handel init_poten (adc1_channel_t *new_channel, gpio_num_t *new_pin, adc_atten_t new_atten, adc_bits_width_t new_width){ //pin/adc, xxx );
    Poten_handel poten = (Poten_handel)malloc(sizeof(Potentiometer_t));    
    if (new_channel == NULL && new_pin == NULL){
        free(poten);
        return NULL;
    }
    if (new_channel != NULL){
        adc1_config_channel_atten(*new_channel, new_atten);
        adc1_config_width(new_width);
        poten->channel = *new_channel;
        poten->atten = new_atten;
        poten->width = new_width;
        poten->pin = *new_channel;
    }
    if (new_pin != NULL){
        adc1_config_channel_atten(*new_pin, new_atten);
        adc1_config_width(new_width);
        poten->channel = *new_pin;
        poten->atten = new_atten;
        poten->width = new_width;
        poten->pin = *new_pin;
    }
    poten->idx = 0;
    return poten;
}

void update_poten(Poten_handel poten){
    int value = adc1_get_raw(poten->channel);
    if (arr_raw_init){
        for(int i=0; i<ARR_RAW_MAX; i++){
            poten->arr_raw[i] = value;
        }
        arr_raw_init = false;
        return;
    }
    poten->arr_raw[poten->idx] = value;
    poten->idx++;
    if (poten->idx == ARR_RAW_MAX){ poten->idx = 0; }
}

static int arr_sum(int* arr, int size){
    int sum = arr[0];
    for (int i=1; i < size; i++){
        sum += arr[i];
    }
    return ((float)sum/size);
}

int getValue_poten(Poten_handel poten){
    int result = arr_sum(poten->arr_raw, ARR_RAW_MAX);
    int prev_result = poten->arr_raw[poten->idx];
    //!hysteria filter, not done
    // if(
    return (int)result;
}

//!Not done
// void setOnThreshold ( int threshold, bool risingEdge){//, void (*onThreshold)(int pin/adc, value) );

// }