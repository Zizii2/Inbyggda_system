#include <stdio.h>
#include "NVS_lib.h"

#define TAG_NVS "NVS_LOG"
#define TAG_MALLOC "MALLOC"
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
    #define SERIAL_VAL_MAX_LEN 8
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

/*
    //TODO free device name (tmp) and serial_name (tmp)
    TODO fix so in Set... the new name is malloc'd and if new name is greater realloc()
*/

static int find_arr_len(char *arr){
    int len = 0;
    for(; arr[len]!='\n'; len++){}
    return ++len;
}

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
    nvs_type_t key;
    err = nvs_find_key(config->nvs_handle, DEVICE_NAME, &key); // use to know if a key exists
    bool found_key = true;
    if (key != NVS_TYPE_STR){
        found_key = false;
    }
    if(found_key){
        // Find device name length
        size_t name_len = 0;
        err = nvs_get_str(config->nvs_handle, DEVICE_NAME, NULL, &name_len);
        if (err != ESP_OK || name_len <= 0 ){ //defaulting device name
            ESP_LOGW(TAG_NVS, "No value found at key [" DEVICE_NAME "]");
            ESP_LOGW(TAG_NVS, "Making a default [" DEVICE_NAME "] = " DEFAULT_DEVICE_NAME);
            ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, DEVICE_NAME, DEFAULT_DEVICE_NAME));
            strcpy(config->device_name, DEFAULT_DEVICE_NAME);
        }
        else if (name_len > DEVICE_NAME_MAX_LEN){
            ESP_LOGW(TAG_NVS, "[" DEVICE_NAME "] has a name longer than %d char, including zero", DEVICE_NAME_MAX_LEN);
            ESP_LOGW(TAG_NVS, "Making a default [" DEVICE_NAME "] = " DEFAULT_DEVICE_NAME);
            ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, DEVICE_NAME, DEFAULT_DEVICE_NAME));    
            strcpy(config->device_name, DEFAULT_DEVICE_NAME);
        }
        else{ // Find device name
            char *device_name = (char *)malloc(name_len * sizeof(char));
            if (device_name == NULL){
                ESP_LOGE(TAG_MALLOC, "Could not malloc space for [" DEVICE_NAME "]");
                return NULL;
            }
            nvs_get_str(config->nvs_handle, DEVICE_NAME, device_name, &name_len);
            strcpy(config->device_name, device_name);
            free(device_name);
        }
        printf("device name lenght: %d\n", name_len);
    }

    found_key = true;
    err = nvs_find_key(config->nvs_handle, SERIAL_NAME, &key); // use to know if a key exists
    if (key != NVS_TYPE_STR){
        found_key = false;
    }
    if (found_key){
        // Find serial number
        size_t serial_len = 0;
        err = nvs_get_str(config->nvs_handle, SERIAL_NAME, NULL, &serial_len);
        if (err != ESP_OK || serial_len <= 0){
            ESP_LOGW(TAG_NVS, "No value found at key [" SERIAL_NAME "]");
            ESP_LOGW(TAG_NVS, "Making a default [" SERIAL_NAME "] = %s", DEFAULT_SERIAL_VAL);
            ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, SERIAL_NAME, DEFAULT_SERIAL_VAL));
            strcpy(config->serial_number, DEFAULT_SERIAL_VAL);
        }
        else if (serial_len > SERIAL_VAL_MAX_LEN){
            ESP_LOGW(TAG_NVS, "[" SERIAL_NAME "] has a value longer than %d char, including zero", SERIAL_VAL_MAX_LEN);
            ESP_LOGW(TAG_NVS, "Making a default [" SERIAL_NAME "] = " DEFAULT_SERIAL_VAL);
            ESP_ERROR_CHECK(nvs_set_str(config->nvs_handle, SERIAL_NAME, DEFAULT_SERIAL_VAL));
            strcpy(config->serial_number, DEFAULT_SERIAL_VAL);

        }
        else{

            char *serial_name = (char *)malloc(serial_len * sizeof(char));
            if (serial_name == NULL){
                ESP_LOGE(TAG_MALLOC, "Could not malloc space for [" SERIAL_NAME "]");
                return NULL;
            }
            nvs_get_str(config->nvs_handle, SERIAL_NAME, serial_name, &serial_len);
            strcpy(config->serial_number, serial_name);
            free(serial_name);
        }
        printf("serial lenght: %d\n", serial_len);
    }
    err = nvs_commit(config->nvs_handle);
    if (err != ESP_OK)
    {
        ESP_LOGE(TAG_NVS, "Failed to commit to nvs");
    }
    nvs_close(config->nvs_handle);

    return config;
}

char *getDeviceName (config_handel config){
    return config->device_name;
}

char *getSerialNumber (config_handel config){
    return config->serial_number;
}

bool setDeviceName (char *new_device_name, config_handel config){
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &config->nvs_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG_NVS, "Could not open" NVS_NAMESPACE);
        return NULL;
    }
    int new_len = find_arr_len(new_device_name);
    if (new_len <= 0){
        ESP_LOGE(TAG_NVS, "New device name is smaller than %d chars", 0);
        ESP_LOGW(TAG_NVS, "Will not set new device name");
        return false;
    }
    if(new_len > DEVICE_NAME_MAX_LEN){
        ESP_LOGE(TAG_NVS, "New device name is larger than %d chars", DEVICE_NAME_MAX_LEN);
        ESP_LOGW(TAG_NVS, "Will not set new device name");
        return false;
    }
    strcpy(config->device_name, new_device_name);

    esp_err_t status_err = nvs_set_str(config->nvs_handle, DEVICE_NAME, new_device_name);
    err = nvs_commit(config->nvs_handle);
    if (err != ESP_OK) { ESP_LOGE(TAG_NVS, "Failed to commit to nvs"); }
    nvs_close(config->nvs_handle);
    if(status_err != ESP_OK || err != ESP_OK){ return false; }
    return true;
}

bool setSerialNumber (char *new_serial_number, config_handel config){
    esp_err_t err = nvs_open(NVS_NAMESPACE, NVS_READWRITE, &config->nvs_handle);
    if (err != ESP_OK){
        ESP_LOGE(TAG_NVS, "Could not open" NVS_NAMESPACE);
        return false;
    }
    int new_len = find_arr_len(new_serial_number);
    if (new_len <= 0){
        ESP_LOGE(TAG_NVS, "New serial number is smaller than %d chars", 0);
        ESP_LOGW(TAG_NVS, "Will not set new serial number");
        return false;
    }
    if(new_len > SERIAL_VAL_MAX_LEN){
        ESP_LOGE(TAG_NVS, "New serial number is larger than %d chars", SERIAL_VAL_MAX_LEN);
        ESP_LOGW(TAG_NVS, "Will not set new serial number");
        return false;
    }
    strcpy(config->serial_number, new_serial_number);

    esp_err_t status_err = nvs_set_str(config->nvs_handle, DEVICE_NAME, new_serial_number);
    err = nvs_commit(config->nvs_handle);
    if (err != ESP_OK){ ESP_LOGE(TAG_NVS, "Failed to commit to NVS"); }
    nvs_close(config->nvs_handle);
    if(status_err != ESP_OK || err != ESP_OK){ return false; }
    return true;
}