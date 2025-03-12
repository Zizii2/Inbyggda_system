#include <stdio.h>
#include <driver/gpio.h>
#include "Step_motor.h"

#define RED GPIO_NUM_14
#define BLUE GPIO_NUM_7
#define GREEN GPIO_NUM_6
#define BLACK GPIO_NUM_8

void app_main(void)
{
    pins_t pins = {
        .pin1 = BLACK,
        .pin2 = GREEN,
        .pin3 = BLUE,
        .pin4 = RED
    };
    turn_cw(&pins);
}