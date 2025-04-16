#include <stdio.h>
#include "HALL_sensor.h"

HALL_handle init_hall(void){
    int pin = 1;
    HALL_handle hall = (HALL_handle)calloc(1, sizeof(HALL_t));
    if(hall == NULL){ return NULL; }
    adc1_config_channel_atten(pin, ADC_ATTEN_DB_12);
    adc1_config_width(ADC_WIDTH_BIT_12);
    hall->buff_idx = 0;
    return hall;
}

int get_avg(HALL_handle hall){
    int avg = 0;
    for(int i=0; i<BUFF_SIZE; i++){
        avg += hall->buffert[i];
    }
    return avg/BUFF_SIZE;
}

void update_hall(HALL_handle hall){
    int value = adc1_get_raw(hall->channel);
    hall->buffert[hall->buff_idx++] = value;
    printf("value: %d\n", get_avg(hall));
}