/* igniter.h */
#ifndef IGNITER
#define IGNITER
#include "driver/gpio.h"

typedef struct {
    gpio_num_t igniter_pin;
    gpio_num_t ingniter_notification_pin;
    float igniter_timer_minutes;
} igniter_args;

void setup_igniter(igniter_args arg);

#endif