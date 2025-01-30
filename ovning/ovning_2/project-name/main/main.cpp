#include <stdio.h>
#include "helper_component.h"
#include "printer.h"

extern "C" void app_main(void){
    func();
    p::print();
}