/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "esp_log.h"
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_flash.h"
#include "esp_system.h"

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"
#define NEW_LINE "\n"

#define PRINTF_COLOR(color, format_string, ...) \
    printf("%s" format_string "%s", color, ##__VA_ARGS__, ANSI_RESET NEW_LINE)

#define PRINTF_GROUP_1(format, ... )\
    PRINTF_COLOR(ANSI_BLUE, "[Group 1] "); \
    PRINTF_COLOR(ANSI_RESET, format, ##__VA_ARGS__)

/*
    debug macros
*/
// __LINE__
//__FILE__
#define PRINTF_COLOR_WITH_LINE(color, format_string, ...) \
    PRINTF_COLOR(ANSI_MAGENTA, "[%s:%d] ", __FILE__, __LINE__); \
    PRINTF_COLOR(color, format_string, ##__VA_ARGS__)


const static char *const TAG = "MAIN";

void app_main(void)
{
    printf(ANSI_RED "Hello" ANSI_RESET " world!\n");
    ESP_LOGI(TAG, "Hello world for my %d time\n", 1);
    //ESP_LOGW (för varningar)
    //ESP_LOGE (för errors)

    const char *test_string = "test";
    printf("%s%s%s",ANSI_RED, test_string, ANSI_RESET NEW_LINE);

    PRINTF_COLOR(ANSI_BLUE, "%s" NEW_LINE, test_string);

    PRINTF_GROUP_1("Hello world %d" NEW_LINE, 1);

    PRINTF_COLOR_WITH_LINE(ANSI_YELLOW, "HEHEHEEHhehehe" NEW_LINE);
















    /* Print chip information */
    esp_chip_info_t chip_info;
    uint32_t flash_size;
    esp_chip_info(&chip_info);
    printf("This is %s chip with %d CPU core(s), %s%s%s%s, ",
           CONFIG_IDF_TARGET,
           chip_info.cores,
           (chip_info.features & CHIP_FEATURE_WIFI_BGN) ? "WiFi/" : "",
           (chip_info.features & CHIP_FEATURE_BT) ? "BT" : "",
           (chip_info.features & CHIP_FEATURE_BLE) ? "BLE" : "",
           (chip_info.features & CHIP_FEATURE_IEEE802154) ? ", 802.15.4 (Zigbee/Thread)" : "");

    unsigned major_rev = chip_info.revision / 100;
    unsigned minor_rev = chip_info.revision % 100;
    printf("silicon revision v%d.%d, ", major_rev, minor_rev);
    if(esp_flash_get_size(NULL, &flash_size) != ESP_OK) {
        printf("Get flash size failed");
        return;
    }

    printf("%" PRIu32 "MB %s flash\n", flash_size / (uint32_t)(1024 * 1024),
           (chip_info.features & CHIP_FEATURE_EMB_FLASH) ? "embedded" : "external");

    printf("Minimum free heap size: %" PRIu32 " bytes\n", esp_get_minimum_free_heap_size());

    for (int i = 10; i >= 0; i--) {
        printf("Restarting in %d seconds...\n", i);
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
    printf("Restarting now.\n");
    fflush(stdout);
    // esp_restart();
}
