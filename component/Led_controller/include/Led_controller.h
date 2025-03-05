// #pragma once
// #include <vector>
// #include <tuple>
// #include <math.h>
// #include <driver/ledc.h>
// #include <driver/gpio.h>
// #include <freertos/FreeRTOS.h>
// #include <freertos/task.h>

// using std::vector;
// using std::tuple;

// enum led_type_t{
//     BINARY,
//     ANALOG
// };

// class led{
// private:
//     int level;
//     bool blinkModeOn;
//     gpio_num_t pin;
//     TickType_t timeOFF;
//     TickType_t timeON;
//     TickType_t lastUpdate;
// public:
//     led(gpio_num_t newPin);
//     void update(void);
//     void setLed(bool toSet);
//     void blinkLed(int new_timeOFF, int new_timeON);
//     gpio_num_t getPin(){ return pin; }
// };

// class analogLed{
// private:
//     gpio_num_t pin;
//     int prevDuty;
//     int currDuty;
//     double period;
//     bool sinModeOn;
// public:
//     analogLed(gpio_num_t newPin);
//     void update(void);
//     void constLed(int newDuty);
//     void waveLed(int period);
//     gpio_num_t getPin(){ return pin; }
// };

// class LedCon{
// private:
//     vector<tuple<gpio_num_t, led_type_t>> listOfLed; 
// public:
//     // Ta in listor på led komponenter och spara
//     LedCon(vector<led> leds, vector<analogLed> analogLeds);

//     // Spela upp animation snake över listorna av leds
//     // Längden på ormen ska vara length och period tiden lapPeriodMs 
//     void snakeAnimation(int length, int lapPeriodMs);

//     void blinkAll(int onMs, int offMs);

//     // Kom på en egen animation du kan spela upp på lamporna
//     void  xxxAnimation(void);
// };



