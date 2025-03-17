#pragma once
enum Step_state{
    ONE,
    TWO,
    THREE,
    FOUR
};

typedef struct Motor_t{
    gpio_num_t pin1;
    gpio_num_t pin2;
    gpio_num_t pin3;
    gpio_num_t pin4;
    enum Step_state state;
    bool turnCW;
} Motor_t;

void turn_cw(Motor_t *motor);

void update_motor(Motor_t *motor);
