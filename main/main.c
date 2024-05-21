/* esp_timer (high resolution timer) example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "esp_timer.h"
#include "esp_log.h"
#include "esp_sleep.h"
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "igniter.h"

#define IGNITE_TIMER_MINUTES 0.1


#define IGNITE_GPIO 5
#define INFO_BLINK_GPIO 2

static const char* TAG = "VOSTOK";

static void mainloop(void);

void app_main(void)
{
    ESP_LOGI(TAG, "Setup Igniter");
    igniter_args vostok_igniter_args = {
        .igniter_pin = IGNITE_GPIO,
        .igniter_timer_minutes = IGNITE_TIMER_MINUTES,
        .ingniter_notification_pin = INFO_BLINK_GPIO
    };

    setup_igniter(&vostok_igniter_args);
    ESP_LOGI(TAG, "Setup Igniter complete");

    gpio_set_level(INFO_BLINK_GPIO, true);
    vTaskDelay(500 / portTICK_PERIOD_MS);
    gpio_set_level(INFO_BLINK_GPIO, false);
    
    mainloop();
}

static void mainloop()
{
    while (1)
    {
        vTaskDelay(10 / portTICK_PERIOD_MS); //<<<Release to the idle background process, we'll be gone from the main loop for min 10mS before the scheduler comes back to us here
    }
}
