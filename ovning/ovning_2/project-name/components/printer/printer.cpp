#include <stdio.h>
#include "printer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define PRINT_COLOR()

void print::printNumbers(int start, int stop)
{
    for(int i=start; i<=stop; i++){
        printf("%d\n", i);
    }
}
void print::takeTimeAndPrintEverySec(int time_limit){
    int time_inc = 1000;
    printf("Wating");
    for(int time_curr = 0; time_curr<time_limit; time_curr+=time_inc){
        printf(".");
        vTaskDelay(pdMS_TO_TICKS(time_inc));
    }
    printf("\n");

}
void print::multiply(int a, int b){
    printf("%d * %d = %d\n",a,b, a * b);
}
