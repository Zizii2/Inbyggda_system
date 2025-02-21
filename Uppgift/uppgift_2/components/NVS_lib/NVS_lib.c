#include <stdio.h>
#include "NVS_lib.h"

#define TAG_NVS "NVS_LOG"
/*
    device name
*/
//device name len
#ifndef DEFAULT_DEVICE_NAME_LEN
    #define DEFAULT_DEVICE_NAME_LEN 12
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

config_handel init_NVS(void){
    config_handel config = (config_handel)malloc(sizeof(configurations_t));
    esp_err_t err = nvs_flash_init();
    if (err == ESP_ERR_NVS_NO_FREE_PAGES || err == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        // NVS partition was truncated and needs to be erased
        // Retry nvs_flash_init
        ESP_ERROR_CHECK(nvs_flash_erase());
        err = nvs_flash_init();
    }
    ESP_ERROR_CHECK(err);

    err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &config->nvs_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG_NVS, "Could not open" NVS_NAMESPACE);
        return NULL;
    }
    // Find device name length
    size_t name_len = 0;
    err = nvs_get_str(config->nvs_handle, DEVICE_NAME, NULL, &name_len);
    if (err != ESP_OK || name_len <= 0){ //defaulting device name
        config->curr_device_name = DEFAULT_DEVICE_NAME;
        ESP_LOGW(TAG_NVS, "Making a default [" DEVICE_NAME "] = " DEFAULT_DEVICE_NAME);
        ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, DEVICE_NAME, DEFAULT_DEVICE_NAME));
    }
    else{ // Find device name
        char *device_name = (char *)malloc(name_len * sizeof(char));
        err = nvs_get_str(config->nvs_handle, DEVICE_NAME, device_name, &name_len);
        if (err != ESP_OK)
        {
            ESP_LOGW(TAG_NVS, "Could not find [" DEVICE_NAME "]");
            ESP_LOGW(TAG_NVS, "Making a default [" DEVICE_NAME "] = " DEFAULT_DEVICE_NAME);
            ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, DEVICE_NAME, DEFAULT_DEVICE_NAME));
            config->curr_device_name = DEFAULT_DEVICE_NAME;
        }
        else{
            config->curr_device_name = device_name;
        }
    }
    // Find serial number
    size_t serial_len = 0;
    err = nvs_get_str(config->nvs_handle, SERIAL_NAME, NULL, &serial_len);
    if (err != ESP_OK || serial_len <= 0){
        config->curr_serial_number = DEFAULT_SERIAL_VAL;
        ESP_LOGW(TAG_NVS, "Could not find [" SERIAL_NAME "]");
        ESP_LOGW(TAG_NVS, "Making a default [" SERIAL_NAME "] = %s", DEFAULT_SERIAL_VAL);
        ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, SERIAL_NAME, DEFAULT_SERIAL_VAL));
    }
    else{
        char *serial_name = (char *)malloc(serial_len * sizeof(char));
        err = nvs_get_str(config->nvs_handle, SERIAL_NAME, serial_name, &serial_len);
        if (err != ESP_OK){
            config->curr_serial_number = DEFAULT_SERIAL_VAL;
            ESP_LOGW(TAG_NVS, "Could not find [" SERIAL_NAME "]");
            ESP_LOGW(TAG_NVS, "Making a default [" SERIAL_NAME "] = %s", DEFAULT_SERIAL_VAL);
            ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, SERIAL_NAME, SERIAL_VAL));
        }
        else{
            config->curr_serial_number = serial_name;
        }
    }
    nvs_commit(config->nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG_NVS, "Failed to commit to nvs");
    }
    nvs_close(config->nvs_handle);

    return config;
}

char *getDeviceName (config_handel config){
    return config->curr_device_name;
}

char *getSerialNumber (config_handel config){

    return config->curr_serial_number;
}

bool setDeviceName (char *new_device_name, config_handel config){
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &config->nvs_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG_NVS, "Could not open" NVS_NAMESPACE);
        return NULL;
    }
    esp_err_t status_err = nvs_set_str(config->nvs_handle, DEVICE_NAME, new_device_name);
    err = nvs_commit(config->nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG_NVS, "Failed to commit to nvs");
    }
    nvs_close(config->nvs_handle);

    if(status_err != ESP_OK){ return false; }
    return true;
}

bool setSerialNumber (char *new_serial_number, config_handel config){
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &config->nvs_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG_NVS, "Could not open" NVS_NAMESPACE);
        return NULL;
    }
    esp_err_t status_err = nvs_set_str(config->nvs_handle, DEVICE_NAME, new_serial_number);
    err = nvs_commit(config->nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG_NVS, "Failed to commit to nvs");
    }
    nvs_close(config->nvs_handle);

    if(status_err != ESP_OK){ return false; }
    return true;
}