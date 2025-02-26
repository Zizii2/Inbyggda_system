#include <stdio.h>
#include "Led_controller.h"

//---------------------------------------------------------
//----------------------- Leds ----------------------------
//---------------------------------------------------------
led::led(gpio_num_t newPin)
: level(0), blinkModeOn(false), pin(newPin), timeOFF(0), timeON(0), lastUpdate(0)
{
    gpio_config_t pinCfg;
    pinCfg.intr_type = GPIO_INTR_DISABLE,
    pinCfg.mode = GPIO_MODE_OUTPUT,
    pinCfg.pin_bit_mask = 1ULL << pin;
    pinCfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
    pinCfg.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&pinCfg);
}

void led::update(void){
    TickType_t currTime = xTaskGetTickCount();
    int newLevel = level;
    if(blinkModeOn && level){ // From on to off
        if(currTime - lastUpdate >= timeON){
            newLevel = 0;
        }
    }
    else if(blinkModeOn && !level){ // From off to on
        if(currTime - lastUpdate >= timeOFF){
            newLevel = 1;
        }
    }
    if(newLevel != level){
        lastUpdate = xTaskGetTickCount();
        gpio_set_level(pin, level);
    }
}

void led::setLed(bool toSet){
    blinkModeOn = false;
    timeOFF = 0;
    timeON = 0;
    if(toSet){level = 1;}
    else{level = 0; }
}

void led::blinkLed(int new_timeOFF, int new_timeON){
    timeOFF = pdMS_TO_TICKS(new_timeOFF);
    timeON = pdMS_TO_TICKS(new_timeON);
    blinkModeOn = true;
    lastUpdate = xTaskGetTickCount();
}

//---------------------------------------------------------
//---------------- Analog Leds ----------------------------
//---------------------------------------------------------
analogLed::analogLed(gpio_num_t newPin)
:   pin(newPin), prevDuty(0), currDuty(0), period(0), sinModeOn(false)
{
    gpio_config_t pinCfg;
    pinCfg.intr_type = GPIO_INTR_DISABLE;
    pinCfg.mode = GPIO_MODE_OUTPUT;
    pinCfg.pin_bit_mask = 1ULL << pin;
    pinCfg.pull_down_en = GPIO_PULLDOWN_ENABLE;
    pinCfg.pull_up_en = GPIO_PULLUP_DISABLE;
    gpio_config(&pinCfg);

    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode       = LEDC_LOW_SPEED_MODE,
        .duty_resolution  = LEDC_TIMER_13_BIT,
        .timer_num        = LEDC_TIMER_0,
        .freq_hz          = 4000,  // Set output frequency at 4 kHz
        .clk_cfg          = LEDC_AUTO_CLK
    };
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));

    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .gpio_num       = pin,
        .speed_mode     = LEDC_LOW_SPEED_MODE,
        .channel        = LEDC_CHANNEL_0,
        .intr_type      = LEDC_INTR_DISABLE,
        .timer_sel      = LEDC_TIMER_0,
        .duty           = 0, // Set duty to 0%
        .hpoint         = 0,
    };
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
}

void analogLed::update(void){
    if(sinModeOn){
        for(int i=0; i<period; i++){
            currDuty = ((sin(i*(2*M_PI)/period) + 1) / 2) * 8192;
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, currDuty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            vTaskDelay(pdMS_TO_TICKS(8));
        }
    }
    else{
        if(currDuty != prevDuty){
            ledc_set_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0, currDuty);
            ledc_update_duty(LEDC_LOW_SPEED_MODE, LEDC_CHANNEL_0);
            prevDuty = currDuty;
        }       
    }

}

void analogLed::constLed(int newDuty){
    prevDuty = currDuty;
    currDuty = newDuty;
    sinModeOn = false;
    period = 0;
}

void analogLed::waveLed(int newPeriod){
    period = newPeriod * 1000;
    sinModeOn = true;
}

//---------------------------------------------------------
//---------------------- ledCon ---------------------------
//---------------------------------------------------------

LedCon::LedCon(vector<led> leds, vector<analogLed> analogLeds){
    for(auto it : leds){
        listOfLed.push_back({it.getPin(), BINARY});
    }
    for(auto it : analogLeds){
        listOfLed.push_back({it.getPin(), ANALOG});
    }
}

void LedCon::snakeAnimation(int length, int lapPeriodMs){

}
void LedCon::blinkAll(int onMs, int offMs){

}
void LedCon::xxxAnimation(){

}