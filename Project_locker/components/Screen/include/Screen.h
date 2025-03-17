#pragma once
typedef struct screen_build{
    bool is_8_bit_flag;
    // int V0_value;
    int RS_pin;
    // int read_write_pin;
    int *DB_arr;
    int enable_pin;
}screen_build;

typedef struct screen_t{
    int V0;
    int RS;
    int row;
    int data_pin;
    int mode;
}screen_t;

typedef screen_t *screen_ptr;

screen_ptr init_screen(screen_build *);
