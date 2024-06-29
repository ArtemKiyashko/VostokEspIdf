/* igniter.h */
#ifndef PRESSUREKEEPALIVEWORKER
#define PRESSUREKEEPALIVEWORKER
#include "driver/gpio.h"

typedef struct {
    gpio_num_t pressure_keepalive_worker_pin;
    uint32_t pressure_keepalive_worker_interval_ms;
} pressure_keepalive_worker_args;

typedef pressure_keepalive_worker_args* pressure_keepalive_worker_args_handle;

void setup_pressure_keepalive_worker(pressure_keepalive_worker_args_handle arg);

#endif