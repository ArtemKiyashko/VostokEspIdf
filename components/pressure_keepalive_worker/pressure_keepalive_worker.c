#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_timer.h"
#include "esp_log.h"
#include "pressure_keepalive_worker.h"

static void setup_pins(pressure_keepalive_worker_args *arg);
static void setup_timers(pressure_keepalive_worker_args *arg);
static void pressure_keepalive_timer_callback(void *arg);

static const char *TAG = "PRESSURE KEEPALIVE WORKER";
esp_timer_handle_t pressure_keepalive_timer = NULL;

void setup_pressure_keepalive_worker(pressure_keepalive_worker_args_handle args)
{
    setup_pins(args);
    setup_timers(args);
    /* Start the timers */
    ESP_LOGI(TAG, "Starting timer with parameters. Pin: %d, Timer interval (ms): %lu",
             args->pressure_keepalive_worker_pin,
             args->pressure_keepalive_worker_interval_ms);
    ESP_ERROR_CHECK(esp_timer_start_periodic(pressure_keepalive_timer, args->pressure_keepalive_worker_interval_ms * 1000));
}

void setup_pins(pressure_keepalive_worker_args *arg)
{
    ESP_LOGI(TAG, "Setup pin");
    // reset pins
    gpio_reset_pin(arg->pressure_keepalive_worker_pin);

    // set pins direction to output
    gpio_set_direction(arg->pressure_keepalive_worker_pin, GPIO_MODE_OUTPUT);

    // set low output level to pin as default state
    gpio_set_level(arg->pressure_keepalive_worker_pin, false);
    ESP_LOGI(TAG, "Setup pin complete");
}

void setup_timers(pressure_keepalive_worker_args *arg)
{
    ESP_LOGI(TAG, "Setup timer");
    const esp_timer_create_args_t pressure_keepalive_timer_args = {
        .callback = &pressure_keepalive_timer_callback,
        .arg = (void *)arg,
        .name = "pressure_keepalive_timer"};

    ESP_ERROR_CHECK(esp_timer_create(&pressure_keepalive_timer_args, &pressure_keepalive_timer));
    ESP_LOGI(TAG, "Setup timers complete");
}

static void pressure_keepalive_timer_callback(void *arg)
{
    pressure_keepalive_worker_args_handle params = (pressure_keepalive_worker_args_handle)arg;
    ESP_LOGI(TAG, "ACTIVATE PRESSURE PIN");
    gpio_set_level(params->pressure_keepalive_worker_pin, true);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(params->pressure_keepalive_worker_pin, false);
}
