#pragma once
#include "nvs_flash.h"
#include "esp_log.h"
#include "string.h"

typedef struct configurations_t{
    char *curr_device_name;
    char *curr_serial_number;
    nvs_handle_t nvs_handle;
} configurations_t;

typedef configurations_t* config_handel;

config_handel init_NVS(void);
// Ladda alla parametrar från NVS

char *getDeviceName (config_handel config);
// Returnera device name parameter från arbetsminne

char *getSerialNumber (config_handel config);
// Returnera serial number parameter från arbetsminne

bool setDeviceName (char * new_device_name, config_handel config);
// Kopiera in nytt device name till arbetsminne och spara på nvs

bool setSerialNumber (char * new_serial_number, config_handel config);
// Kopiera in nytt serial number till arbetsminne och spara på nvs


