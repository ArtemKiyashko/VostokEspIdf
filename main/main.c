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

void app_main(void)
{
    ESP_LOGI(TAG, "SETUP IGNITER");
    igniter_args vostok_igniter_args = {
        .igniter_pin = 5,
        .igniter_timer_minutes = 0.1,
        .ingniter_notification_pin = 2
    };

    setup_igniter(vostok_igniter_args);
    ESP_LOGI(TAG, "SETUP IGNITER COMPLETE");   
}


