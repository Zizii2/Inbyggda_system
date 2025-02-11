#include <stdarg.h>
#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

typedef void(*callback)(int);

typedef struct button_t{
    int latch;
    int level;
    gpio_num_t btn_pin;
    callback cb;
} button_t;

typedef button_t* button_handel;

button_handel init(gpio_int_type_t new_intr, gpio_mode_t new_mode, gpio_pullup_t new_pull_up, gpio_pulldown_t new_pull_down, gpio_num_t new_pin);

void update(button_handel btn, TickType_t elapsed_time);

bool isPressed(int level);

void setOnPressed(void (*onPressed)(int));