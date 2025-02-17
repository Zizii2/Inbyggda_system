#include <stdio.h>
#include "Potentiometer.h"

#define get_avg(sum, count) ((float)sum/count)

static bool arr_raw_init = true;
static bool intr_allowed = false;

static int arr_sum(int* arr, int size);
static void IRAM_ATTR gpio_isr_handler(void *arg){
    bool *do_callback = (bool *)arg;
    *do_callback = RISE_EN_POTEN;
}

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
    poten->cb = NULL;
    poten->data = 0;
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
    int aprox;
    // printf("%d\n", poten->do_callback);
    switch (poten->do_callback)
    {
    case THRES_EN_POTEN:
        aprox = get_avg(arr_sum(poten->arr_raw, ARR_RAW_MAX), ARR_RAW_MAX);
        if (aprox >= poten->threshold){
            poten->cb(poten->channel, poten->data);
            poten->do_callback = THRES_DIS_POTEN;
        }
        break;
    case THRES_DIS_POTEN:
        aprox = get_avg(arr_sum(poten->arr_raw, ARR_RAW_MAX), ARR_RAW_MAX);
        if (aprox < poten->threshold){
            poten->do_callback = THRES_EN_POTEN;
        }
        break;
    case RISE_EN_POTEN:
        poten->cb(poten->channel, poten->data);
        poten->do_callback = RISE_DIS_POTEN;
        gpio_isr_handler_remove(poten->pin);
        intr_allowed = true;
        break;
    case RISE_DIS_POTEN:
        if (intr_allowed){
            printf("hello\n");
            intr_allowed = false;
            gpio_isr_handler_add(poten->pin, gpio_isr_handler, (void *)poten->do_callback);
        }
        break;
    default:
        break;
    }
    poten->idx++;
    if (poten->idx == ARR_RAW_MAX){ poten->idx = ARR_RAW_START; }
}

int getValue_poten(Poten_handel poten){
    int sum = arr_sum(poten->arr_raw, ARR_RAW_MAX);
    poten->prev_sum = sum;
    return get_avg(sum, ARR_RAW_MAX);
}

void setOnThreshold_poten(Poten_handel poten, int threshold, bool on_rising_edge, void (*onThreshold)(adc1_channel_t channel, int value), int data){
    if((threshold > 0 && on_rising_edge) || (threshold <= 0 && !on_rising_edge)){ ESP_ERROR_CHECK(ESP_ERR_INVALID_ARG); }
    if(on_rising_edge){
        printf("Trying rise edge\n");
        gpio_config_t new_intr_config = {
            .intr_type = GPIO_INTR_POSEDGE,
            .pin_bit_mask = 1ULL << poten->pin,
            .pull_down_en = GPIO_PULLDOWN_DISABLE,
            .pull_up_en = GPIO_PULLUP_DISABLE,
            // .mode = GPIO_MODE_INPUT
        };
        gpio_config(&new_intr_config);
        gpio_install_isr_service(0);
        // gpio_isr_handler_add(poten->pin, gpio_isr_handler, (void *)poten->do_callback);
        poten->do_callback = RISE_DIS_POTEN;
        intr_allowed = true;
    }
    else{
        poten->threshold = threshold;
        poten->do_callback = THRES_EN_POTEN;
    }
    poten->cb = onThreshold;
    poten->data = data;
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