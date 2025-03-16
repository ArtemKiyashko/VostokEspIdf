#include <stdio.h>
#include <inttypes.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_chip_info.h"
#include "esp_system.h"
#include <ms5611.h>
#include <esp_log.h>
#include "igniter.h"

//comment this line to enable desp32 devkit mode
#define PRODUCTION_MODE

#define IGNITE_TIMER_MINUTES 0.5
#define IGNITE_PRESSURE_THRESHOLD_PA 1197 //30000 meters
#define IGNITE_GPIO GPIO_NUM_23
#define PRESSURE_SENSOR_SDA GPIO_NUM_21
#define PRESSURE_SENSOR_SCL GPIO_NUM_22

#if defined(PRODUCTION_MODE)
/*GPIO 2 for ESP32, GPIO 16 for WEMOS ESP32 programmable led*/
// #define INFO_BLINK_GPIO 2
#define INFO_BLINK_GPIO GPIO_NUM_16
#define INFO_BLINK_DIRECT false
#else
#define INFO_BLINK_GPIO GPIO_NUM_2
#define INFO_BLINK_DIRECT true
#endif

static const char *TAG = "VOSTOK";
void blink_info_times(uint8_t times);
void pressureSensorTask(void *pvParameters);

void app_main(void)
{
    ESP_ERROR_CHECK(i2cdev_init());
    xTaskCreatePinnedToCore(pressureSensorTask, "pressureSensorTask", configMINIMAL_STACK_SIZE * 8, NULL, 5, NULL, APP_CPU_NUM);
}

void pressureSensorTask(void *pvParameters)
{
    ms5611_t pressureSensor = { 0 };
    bool igniter_triggered = false;

    ESP_ERROR_CHECK(ms5611_init_desc(&pressureSensor, MS5611_ADDR_CSB_LOW, I2C_NUM_0, PRESSURE_SENSOR_SDA, PRESSURE_SENSOR_SCL));
    ESP_ERROR_CHECK(ms5611_init(&pressureSensor, MS5611_OSR_1024));

    blink_info_times(3);

    float temperature;
    int32_t pressure;
    esp_err_t res;

    while (1)
    {
        vTaskDelay(pdMS_TO_TICKS(500));
        res = ms5611_get_sensor_data(&pressureSensor, &pressure, &temperature);
        if (res != ESP_OK)
        {
            ESP_LOGI(TAG, "Temperature/pressure reading failed: %d (%s)", res, esp_err_to_name(res));
            continue;
        }
        ESP_LOGI(TAG, "Pressure: %lu Pa, Temperature: %.2f C\n", pressure, temperature);
        if (pressure < IGNITE_PRESSURE_THRESHOLD_PA && !igniter_triggered)
        {
            ESP_LOGI(TAG, "Pressure below threshold. Setup Igniter");
            igniter_args vostok_igniter_args = {
                .igniter_pin = IGNITE_GPIO,
                .igniter_timer_minutes = IGNITE_TIMER_MINUTES,
                .ingniter_notification_pin = INFO_BLINK_GPIO,
                .info_blink_direct = INFO_BLINK_DIRECT};
        
            setup_igniter(&vostok_igniter_args);
            ESP_LOGI(TAG, "Setup Igniter complete");
            igniter_triggered = true;
        }
    }
}

void blink_info_times(uint8_t times)
{
    gpio_reset_pin(INFO_BLINK_GPIO);
    gpio_set_direction(INFO_BLINK_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(INFO_BLINK_GPIO, !INFO_BLINK_DIRECT);

    bool pinstatus = !INFO_BLINK_DIRECT;

    for (int i = 0; i < times * 2; i++)
    {
        pinstatus = !pinstatus;
        gpio_set_level(INFO_BLINK_GPIO, pinstatus);
        vTaskDelay(500 / portTICK_PERIOD_MS);
    }
}


