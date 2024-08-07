#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_log.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "igniter.h"
#include "pressure_keepalive_worker.h"

#define IGNITE_TIMER_MINUTES 120

#define IGNITE_GPIO 23
/*GPIO 2 for ESP32, GPIO 16 for WEMOS ESP32 programmable led*/
// #define INFO_BLINK_GPIO 2
#define INFO_BLINK_GPIO 16
#define PRESSURE_WORKER_PIN 22
#define PRESSURE_WORKER_INTERVAL_MS 10000 

static const char *TAG = "VOSTOK";
static void mainloop(void);
static void blink_info_times(uint8_t times);

void app_main(void)
{
    ESP_LOGI(TAG, "Setup Igniter");
    igniter_args vostok_igniter_args = {
        .igniter_pin = IGNITE_GPIO,
        .igniter_timer_minutes = IGNITE_TIMER_MINUTES,
        .ingniter_notification_pin = INFO_BLINK_GPIO};

    setup_igniter(&vostok_igniter_args);
    ESP_LOGI(TAG, "Setup Igniter complete");

    blink_info_times(3);

    mainloop();
}

static void blink_info_times(uint8_t times)
{
    bool pinstatus = true;

    for (int i = 0; i < times * 2; i++)
    {
        pinstatus = !pinstatus;
        gpio_set_level(INFO_BLINK_GPIO, pinstatus);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}

static void mainloop()
{
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS); //<<<Release to the idle background process, we'll be gone from the main loop for min 10mS before the scheduler comes back to us here
    }
}
