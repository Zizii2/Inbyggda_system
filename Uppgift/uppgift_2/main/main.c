#include <stdio.h>
#include "NVS_lib.h"

void hej(config_handel config){
    char name[] = "test";
    if (setDeviceName(name, config)){
        printf("device ok\n");
        printf("device name: %s\n", getDeviceName(config));
    }

}

void app_main(void)
{
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
}