#include <stdio.h>
#include "helper_component.h"
#include "printer.h"

using namespace print;

extern "C" void app_main(void){
    // func();
    printNumbers(0, 10);
    takeTimeAndPrintEverySec(5000);
    multiply(10, 5);
}