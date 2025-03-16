/* igniter.h */
#ifndef IGNITER
#define IGNITER
#include "driver/gpio.h"

typedef struct {
    gpio_num_t igniter_pin;
    gpio_num_t ingniter_notification_pin;
    float igniter_timer_minutes;
    bool info_blink_direct;
} igniter_args;

typedef igniter_args* igniter_args_handle;

void setup_igniter(igniter_args_handle args);

#endif