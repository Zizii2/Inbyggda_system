#include <stdio.h>
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <string>
#include <iostream>
#include "esp_log.h"

#define ANSI_RED "\033[0;31m"
#define ANSI_GREEN "\033[0;32m"
#define ANSI_YELLOW "\033[0;33m"
#define ANSI_BLUE "\033[0;34m"
#define ANSI_MAGENTA "\033[0;35m"
#define ANSI_CYAN "\033[0;36m"
#define ANSI_RESET "\033[0m"
#define NEW_LINE "\n"

#define PRINT_COLOR(color, format_str, ...) \
    printf("%s" format_str "%s", color, ##__VA_ARGS__, ANSI_RESET)

#define PRINT_W_TAG(tag_color, tag_name, format_str, ...) \
    PRINT_COLOR(tag_color, "[%s] ", tag_name); \
    PRINT_COLOR(ANSI_RESET, format_str, ##__VA_ARGS__)

void print::printNumbers(int start, int stop)
{
    for(int i=start; i<=stop; i++){
        PRINT_W_TAG(ANSI_BLUE, "Nr.", "%d\n", i);
    }
}
void print::takeTimeAndPrintEverySec(int time_limit){
    const int time_inc = 1000;
    printf("Wating");
    for(int time_curr = 0; time_curr<time_limit; time_curr+=time_inc){
        printf("\n.");
        if(time_curr + time_inc <= time_limit){
            vTaskDelay(pdMS_TO_TICKS(time_inc));
        }
        else{
            vTaskDelay(pdMS_TO_TICKS(time_limit - time_curr));
        }
    }
    printf("\n");
}
void print::multiply(int a, int b){
    printf("%d * %d = %d\n",a,b, a * b);
}
