// #include <stdio.h>
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "esp_system.h"
// #include "esp_log.h"
// #include "driver/uart.h"
// #include "string.h"
// #include "driver/gpio.h"

// static const int RX_BUF_SIZE = 1024;
// static const int TX_BUF_SIZE = 0;

// #define TXD_PIN (GPIO_NUM_19)
// #define RXD_PIN (GPIO_NUM_20)

// static void init(void){
//     const uart_config_t uart_config = {
//         .baud_rate = 115200,
//         .data_bits = UART_DATA_8_BITS,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
//         .parity = UART_PARITY_DISABLE,
//         .stop_bits = UART_STOP_BITS_1,
//         .source_clk = UART_SCLK_DEFAULT
//     };
//     uart_driver_install(UART_NUM_1, RX_BUF_SIZE * 2, TX_BUF_SIZE, 0, NULL, 0);
//     uart_param_config(UART_NUM_1, &uart_config);
//     uart_set_pin(UART_NUM_1, TXD_PIN, RXD_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);

// }

// int sendData(const char* logName, const char* data)
// {
//     const int len = strlen(data);
//     const int txBytes = uart_write_bytes(UART_NUM_1, data, len);
//     ESP_LOGI(logName, "Wrote %d bytes", txBytes);
//     return txBytes;
// }

// static void tx_task(void *arg)
// {
//     static const char *TX_TASK_TAG = "TX_TASK";
//     esp_log_level_set(TX_TASK_TAG, ESP_LOG_INFO);
//     while (1) {
//         sendData(TX_TASK_TAG, "Hello world");
//         vTaskDelay(2000 / portTICK_PERIOD_MS);
//     }
// }

// static void rx_task(void *arg)
// {
//     static const char *RX_TASK_TAG = "RX_TASK";
//     esp_log_level_set(RX_TASK_TAG, ESP_LOG_INFO);
//     uint8_t* data = (uint8_t*) malloc(RX_BUF_SIZE + 1);
//     while (1) {
//         const int rxBytes = uart_read_bytes(UART_NUM_1, data, RX_BUF_SIZE, 1000 / portTICK_PERIOD_MS);
//         printf("%d\n", rxBytes);
//         if (rxBytes > 0) {
//             data[rxBytes] = 0;
//             ESP_LOGI(RX_TASK_TAG, "Read %d bytes: '%s'", rxBytes, data);
//             ESP_LOG_BUFFER_HEXDUMP(RX_TASK_TAG, data, rxBytes, ESP_LOG_INFO);
//         }
//     }
//     free(data);
// }


// void app_main(void)
// {
//     init();
//     xTaskCreate(rx_task, "uart_rx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 1, NULL);
//     xTaskCreate(tx_task, "uart_tx_task", 1024 * 2, NULL, configMAX_PRIORITIES - 2, NULL);
// }


#include "driver/i2c_slave.h"
#include "driver/i2c_master.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"

static const char *TAG = "example";

#define I2C_SLAVE_SCL_IO (GPIO_NUM_20) // SCL
#define I2C_SLAVE_SDA_IO (GPIO_NUM_19) // SDA

typedef struct {
    QueueHandle_t event_queue;
    uint8_t command_data;
    i2c_slave_dev_handle_t handle;
} i2c_slave_context_t;

typedef enum {
    I2C_SLAVE_EVT_RX,
    I2C_SLAVE_EVT_TX
} i2c_slave_event_t;

static esp_err_t event_handler(esp_client_event_t *evt)
{
    i2c_slave_github_context_t *context = (i2c_slave_github_context_t *)evt->user_data;
    int star_count = 0, forks_count = 0, open_issues_count = 0;

    switch (evt->event_id) {
    case HTTP_EVENT_ON_DATA:
        if (evt->data_len > 0) {
            if (context->json_buffer == NULL) {
                context->json_buffer = malloc(evt->data_len + 1);
            } else {
                context->json_buffer = realloc(context->json_buffer, context->json_size + evt->data_len + 1);
            }
            if (context->json_buffer == NULL) {
                ESP_LOGE("HTTP_CLIENT", "Failed to allocate memory for data json_buffer");
                return ESP_FAIL;
            }
            memcpy(context->json_buffer + context->json_size, evt->data, evt->data_len);
            context->json_size += evt->data_len;
            context->json_buffer[context->json_size] = '\0';  // Null-terminate the string
        }
        break;
    case HTTP_EVENT_ON_FINISH:
        if (context->json_buffer != NULL) {
            // Process received data
            cJSON *root = cJSON_Parse(context->json_buffer);
            cJSON *stars = cJSON_GetObjectItem(root, "stargazers_count");

            if (stars != NULL) {
                star_count = stars->valueint;
                printf("Star count: %d\n", star_count);
                memcpy(context->tmp_buffer_stars, &star_count, sizeof(int));
            }
            cJSON *forks = cJSON_GetObjectItem(root, "forks_count");
            if (forks != NULL) {
                forks_count = forks->valueint;
                printf("Forks count: %d\n", forks_count);
                memcpy(context->tmp_buffer_forks, &forks_count, sizeof(int));
            }
            cJSON *open_issues = cJSON_GetObjectItem(root, "open_issues_count");
            if (open_issues != NULL) {
                open_issues_count = open_issues->valueint;
                printf("issue count: %d\n", open_issues_count);
                memcpy(context->tmp_buffer_open_issues, &open_issues_count, sizeof(int));
            }
            cJSON *descriptions = cJSON_GetObjectItem(root, "description");
            if (descriptions != NULL) {
                printf("the description is: %s\n", descriptions->valuestring);
                memcpy(context->tmp_buffer_descriptions, descriptions->valuestring, strlen(descriptions->valuestring));
            }
            cJSON_Delete(root);
            free(context->json_buffer);
            context->json_buffer = NULL;
            context->json_size = 0;
        }
        break;
    default:
        break;
    }

    return ESP_OK;
}

static bool i2c_slave_request_cb(i2c_slave_dev_handle_t i2c_slave, void *arg)
{
    i2c_slave_context_t *context = (i2c_slave_context_t *)arg;
    i2c_slave_event_t evt = I2C_SLAVE_EVT_TX;
    BaseType_t xTaskWoken = 0;
    xQueueSendFromISR(context->event_queue, &evt, &xTaskWoken);
    return xTaskWoken;
}

static bool i2c_slave_receive_cb(i2c_slave_dev_handle_t i2c_slave, void *arg)
{
    i2c_slave_context_t *context = (i2c_slave_context_t *)arg;
    i2c_slave_event_t evt = I2C_SLAVE_EVT_RX;
    BaseType_t xTaskWoken = 0;
    // Command only contains one byte, so just save one bytes here.
    context->command_data = *evt_data->buffer;
    xQueueSendFromISR(context->event_queue, &evt, &xTaskWoken);
    return xTaskWoken;
}

void app_main(void){
    static i2c_slave_context_t context = {0};
    ESP_ERROR_CHECK(esp_event_loop_create_default());

    context.event_queue = xQueueCreate(16, sizeof(i2c_slave_event_t));
    if (!context.event_queue) {
        ESP_LOGE(TAG, "Creating queue failed");
        return;
    }
    i2c_slave_config_t slave_config = {
        .i2c_port = -1, // -1 is for auto selecting port
        .clk_source = I2C_CLK_SRC_DEFAULT,
        .scl_io_num = I2C_SLAVE_SCL_IO,
        .sda_io_num = I2C_SLAVE_SDA_IO,
        .slave_addr = 1,
        .send_buf_depth = 100,
        .receive_buf_depth = 100
    };

    i2c_slave_dev_handle_t slave_handle;
    ESP_ERROR_CHECK(i2c_new_slave_device(&slave_config, &slave_handle));
    context.handle = slave_handle;

    // Register callback in a task
    i2c_slave_event_callbacks_t cbs = {
        .on_receive = i2c_slave_receive_cb,
        .on_request = i2c_slave_request_cb,
    };
    ESP_ERROR_CHECK(i2c_slave_register_event_callbacks(context.handle, &cbs, &context));

    while(1){
        
    }
}