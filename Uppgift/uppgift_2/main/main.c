#include <stdio.h>
#include "NVS_lib.h"

void app_main(void)
{
    config_handel config = init_NVS();
    printf("device name: %s\n", getDeviceName(config));
    printf("Serial number: %s\n", getSerialNumber(config));
    if (setDeviceName("Anton's device", config)){
        printf("device ok\n");
    }
    if (setSerialNumber("0x00123ABC", config)){
        printf("serial ok\n");
    }

//     printf("device name: %s\n", getDeviceName(config));
//     printf("Serial number: %s\n", getSerialNumber(config));
}