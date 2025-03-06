#pragma once
#include "nvs_flash.h"
#include "esp_log.h"
#include "string.h"

/*
    device name
*/
//device name len
#ifndef DEFAULT_DEVICE_NAME_LEN
    #define DEFAULT_DEVICE_NAME_LEN 12
#endif

// device name max length
#ifndef DEVICE_NAME_MAX_LEN
    #define DEVICE_NAME_MAX_LEN 50
#endif

//default device name key
#ifndef DEFAULT_DEVICE_NAME
    #define DEFAULT_DEVICE_NAME "Device_name"
#endif

//device name key
#ifndef DEVICE_NAME
        #define DEVICE_NAME DEFAULT_DEVICE_NAME
#endif

/*
    serial
*/
//serial len
#ifndef DEFAULT_SERIAL_LEN
    #define DEFAULT_SERIAL_NR 7
#endif

//default serial name
#ifndef DEFAULT_SERIAL_NAME
    #define DEFAULT_SERIAL_NAME "Serial"
#endif

//serial key
#ifndef SERIAL_NAME
    #define SERIAL_NAME DEFAULT_SERIAL_NAME
#endif

//default serial value
#ifndef DEFAULT_SERIAL_VAL
    #define DEFAULT_SERIAL_VAL "0000000"
#endif

#ifndef SERIAL_VAL_MAX_LEN
    #define SERIAL_VAL_MAX_LEN 9
#endif

//serial value
#ifndef SERIAL_VAL
    #define SERIAL_VAL DEFAULT_SERIAL_VAL
#endif

/*
    namespace
*/
#ifndef NVS_NAMESPACE
    #define NVS_NAMESPACE "Device_config"
#endif


typedef struct configurations_t{
    char device_name[DEVICE_NAME_MAX_LEN];
    char serial_number[SERIAL_VAL_MAX_LEN];
    nvs_handle_t nvs_handle;
} configurations_t;

typedef configurations_t* config_handel;

config_handel init_NVS(void);
// Ladda alla parametrar från NVS

char *getDeviceName (config_handel config);
// Returnera device name parameter från arbetsminne

char *getSerialNumber (config_handel config);
// Returnera serial number parameter från arbetsminne

bool setDeviceName (char *new_device_name, config_handel config);
// Kopiera in nytt device name till arbetsminne och spara på nvs

bool setSerialNumber (char *new_serial_number, config_handel config);
// Kopiera in nytt serial number till arbetsminne och spara på nvs
