#include <stdio.h>
#include "NVS_lib.h"
#include "Led_controller.h"

void hej(config_handel config){
    char name[] = "test";
    if (setDeviceName(name, config)){
        printf("device ok\n");
        printf("device name: %s\n", getDeviceName(config));
    }

}

extern "C" void app_main(void)
{
    analogLed led1(GPIO_NUM_5);
    led1.waveLed(10);
    while(1){
        led1.update();
        vTaskDelay(pdMS_TO_TICKS(30));
    }
}

/*
config_handel config = init_NVS();
    printf("device name: %s\n", getDeviceName(config));
    printf("Serial number: %s\n", getSerialNumber(config));
    hej(config);
    printf("device name: %s\n", getDeviceName(config));
    if (setDeviceName("Anton's device", config)){
        printf("device ok\n");
        printf("device name: %s\n", getDeviceName(config));
    }
    if (setSerialNumber("0x001", config)){
        printf("serial ok\n");
        printf("Serial number: %s\n", getSerialNumber(config));
    }
*/