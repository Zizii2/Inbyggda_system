#include <stdio.h>
#include "Potentiometer.h"

#define get_avg(sum, count) ((float)sum/count)

static bool arr_raw_init = true;

static int arr_sum(int* arr, int size);

Poten_handel init_poten (Potentiometer_config_t *poten_config){
    if((poten_config->use_channel && poten_config->use_pin) || (!poten_config->use_channel && !poten_config->use_pin)){
        return NULL;
    }
    Poten_handel poten = (Poten_handel)malloc(sizeof(Potentiometer_t)); 
    if (poten_config->use_channel){
        adc1_config_channel_atten(poten_config->channel, poten_config->atten);
        poten->channel = poten_config->channel;
        poten->pin = poten_config->channel;
    }
    else{
        adc1_config_channel_atten(poten_config->pin, poten_config->atten);
        poten->channel = poten_config->pin;
        poten->pin = poten_config->pin;
    }
    adc1_config_width(poten_config->width);
    poten->atten = poten_config->atten;
    poten->width = poten_config->width;
    poten->idx = ARR_RAW_START;
    poten->prev_sum = 0;
    poten->threshold = 0;
    poten->on_rising_edge = false;
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
    if (poten->idx == ARR_RAW_MAX){ poten->idx = ARR_RAW_START; }
}

int getValue_poten(Poten_handel poten, int data){
    int sum = arr_sum(poten->arr_raw, ARR_RAW_MAX);
    if (get_avg(sum, ARR_RAW_MAX) > get_avg(poten->prev_sum, ARR_RAW_MAX) && poten->on_rising_edge){
        poten->cb(poten->channel, data);
    }
    else if (sum >= poten->threshold){
        poten->cb(poten->channel, data);
    }
    poten->prev_sum = sum;
    return get_avg(sum, ARR_RAW_MAX);
}

//!Not done
void setOnThreshold_poten(Poten_handel poten, int threshold, bool on_rising_edge, void (*onThreshold)(adc1_channel_t channel, int value)){
    poten->threshold = threshold;
    poten->on_rising_edge = on_rising_edge;
    poten->cb = onThreshold;
}

static int arr_sum(int* arr, int size){
    int sum = arr[ARR_RAW_START];
    for (int i=ARR_RAW_START+1; i < size; i++){
        sum += arr[i];
    }
    return sum;
}






// //!hysteria filter, not done
// if (poten->arr_raw[poten->idx-1] != poten->arr_raw[poten->idx] && poten->arr_raw[poten->idx] - poten->arr_raw[poten->idx-1] == 1){
//     int prev_raw = poten->arr_raw[poten->idx-1];
//     int treshold = prev_raw * ARR_RAW_MAX - 6;
//     if (treshold > sum){

// }
// else if (poten->arr_raw[poten->idx+1] != poten->arr_raw[poten->idx] && poten->arr_raw[poten->idx+1] - poten->arr_raw[poten->idx] == 1){

// }
// // if (