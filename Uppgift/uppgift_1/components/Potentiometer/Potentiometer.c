#include <stdio.h>
#include "Potentiometer.h"

#define get_avg(sum, count) ((float)sum/count)

static bool arr_raw_init = true;

static int arr_sum(int* arr, int size);
static void callback_managment(Poten_handel poten);

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
    poten->idx = ARR_RAW_START;
    poten->prev_sum = 0;
    poten->threshold = 0;
    poten->on_rising_edge = false;
    poten->do_callback = NOT_INIT_POTEN;
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
    if (poten->do_callback){
        callback_managment(poten);
    }
    poten->idx++;
    if (poten->idx == ARR_RAW_MAX){ poten->idx = ARR_RAW_START; }
}

int getValue_poten(Poten_handel poten){
    int sum = arr_sum(poten->arr_raw, ARR_RAW_MAX);
    poten->prev_sum = sum;
    return get_avg(sum, ARR_RAW_MAX);
}

void setOnThreshold_poten(Poten_handel poten, int threshold, bool on_rising_edge, void (*onThreshold)(adc1_channel_t channel, void *value), void *data){
    poten->threshold = threshold;
    poten->on_rising_edge = on_rising_edge;
    if (onThreshold <= 0){ onThreshold = 4000; } //? how do I make this more modulare?
    poten->cb = onThreshold;
    poten->data = data;
    poten->do_callback = ENABLED_POTEN;
}

static int arr_sum(int* arr, int size){
    int sum = arr[ARR_RAW_START];
    for (int i=ARR_RAW_START+1; i < size; i++){
        sum += arr[i];
    }
    return sum;
}

static void callback_managment(Poten_handel poten){
    int count_for_rising_edge = 2;
    int prev_idx = poten->idx - 1;
    int has_rising_edge = count_for_rising_edge;
    for(int i=ARR_RAW_START; i<count_for_rising_edge; i++){
        if (prev_idx < ARR_RAW_START){
            prev_idx = ARR_RAW_MAX - 1;
        }
        if (poten->arr_raw[poten->idx] < poten->arr_raw[prev_idx]){
            prev_idx--;
        }
    }
    if (has_rising_edge && poten->do_callback == ENABLED_POTEN && poten->on_rising_edge){ //if not == 0 then it is enough of a rising edge
        poten->cb(poten->channel, poten->data);
        poten->do_callback = DISABLED_POTEN;
    }
    else if (!has_rising_edge && poten->on_rising_edge){
        poten->do_callback = ENABLED_POTEN;
    }
    if (poten->arr_raw[poten->idx] >= poten->threshold && poten->do_callback == ENABLED_POTEN){
        poten->cb(poten->channel, poten->data);
        poten->do_callback = DISABLED_POTEN;
    }
    else if (poten->arr_raw[poten->idx] < poten->threshold) {
        poten->do_callback = ENABLED_POTEN;
    }
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