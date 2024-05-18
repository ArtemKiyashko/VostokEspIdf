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

#define IGNITE_TIMER_MINUTES 0.1
#define IGNITE_GPIO 5
#define INFO_BLINK_GPIO 2


static void ignite_timer_callback(void* arg);

static const char* TAG = "VOSTOK";

void app_main(void)
{
    ESP_LOGI(TAG, "SETUP PINS");
    gpio_reset_pin(IGNITE_GPIO);
    gpio_reset_pin(INFO_BLINK_GPIO);

    gpio_set_direction(IGNITE_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_direction(INFO_BLINK_GPIO, GPIO_MODE_OUTPUT);

    gpio_set_level(INFO_BLINK_GPIO, false);
    gpio_set_level(IGNITE_GPIO, false);

    ESP_LOGI(TAG, "SETUP PINS COMPLETE");

    ESP_LOGI(TAG, "SETUP TIMERS");
    const esp_timer_create_args_t ignite_timer_args = {
            .callback = &ignite_timer_callback,
            .arg = NULL,
            .name = "ignite"
    };
    esp_timer_handle_t ignite_timer;
    ESP_ERROR_CHECK(esp_timer_create(&ignite_timer_args, &ignite_timer));

    /* Start the timers */
    ESP_ERROR_CHECK(esp_timer_start_once(ignite_timer, IGNITE_TIMER_MINUTES * 60 * 1000000));

    ESP_LOGI(TAG, "SETUP TIMERS COMPLETE");
}

static void ignite_timer_callback(void* arg)
{
    gpio_set_level(INFO_BLINK_GPIO, true);
    gpio_set_level(IGNITE_GPIO, true);

    vTaskDelay(100 / portTICK_PERIOD_MS);

    gpio_set_level(IGNITE_GPIO, false);
    gpio_set_level(INFO_BLINK_GPIO, false);
}
