#include <stdio.h>
#include "Button.h"

void app_main(void)
{
    btn_handel btn1 = init(GPIO_INTR_DISABLE, GPIO_MODE_INPUT, GPIO_PULLUP_DISABLE, GPIO_PULLDOWN_DISABLE, GPIO_NUM_10);
}