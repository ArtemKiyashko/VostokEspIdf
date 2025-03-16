/* igniter.c */
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "igniter.h"
#include "esp_timer.h"
#include "esp_log.h"

#define MINIMAL_TIMER_VALUE_MINUTES 0.5

static void setup_pins(igniter_args *arg);
static void setup_timers(igniter_args *arg);
static void ignite_timer_callback(void *arg);
static void preignite_notification_callback(void *arg);
static void validate(igniter_args *arg);
static uint64_t toUseconds(float minutes);

esp_timer_handle_t ignite_timer = NULL;
esp_timer_handle_t preignite_notification_timer = NULL;

static const char *TAG = "IGNITER";

void setup_igniter(igniter_args_handle args)
{
    validate(args);
    setup_pins(args);
    setup_timers(args);

    /* Start the timers */
    ESP_LOGI(TAG, "Starting timers with parameters. Igniter pin: %d, Igniter timer minutes: %f, Igniter notification pin: %d",
             args->igniter_pin,
             args->igniter_timer_minutes,
             args->ingniter_notification_pin);
    ESP_ERROR_CHECK(esp_timer_start_once(preignite_notification_timer, toUseconds(args->igniter_timer_minutes) - 10 * 1000000 /*10 seconds before ignite*/));
    ESP_ERROR_CHECK(esp_timer_start_once(ignite_timer, toUseconds(args->igniter_timer_minutes)));
}

#pragma region private members
static uint64_t toUseconds(float minutes)
{
    return minutes * 60 * 1000000;
}

static void validate(igniter_args *arg)
{
    // do not allow values less than 30 seconds due to safety!
    if (arg->igniter_timer_minutes < MINIMAL_TIMER_VALUE_MINUTES)
    {
        ESP_LOGI(TAG, "Timer value %f minutes less than minimal. Setting minimal value %f minutes", arg->igniter_timer_minutes, MINIMAL_TIMER_VALUE_MINUTES);
        arg->igniter_timer_minutes = MINIMAL_TIMER_VALUE_MINUTES;
    }
}
static void setup_pins(igniter_args *arg)
{
    ESP_LOGI(TAG, "Setup pins");
    // reset igniter and notification pins
    gpio_reset_pin(arg->igniter_pin);
    gpio_reset_pin(arg->ingniter_notification_pin);

    // set pins direction to output
    gpio_set_direction(arg->igniter_pin, GPIO_MODE_OUTPUT);
    gpio_set_direction(arg->ingniter_notification_pin, GPIO_MODE_OUTPUT);

    // set low output level to pins as default state
    gpio_set_level(arg->igniter_pin, false);
    gpio_set_level(arg->ingniter_notification_pin, !arg->info_blink_direct);
    ESP_LOGI(TAG, "Setup pins complete");
}

static void setup_timers(igniter_args *arg)
{
    ESP_LOGI(TAG, "Setup timers");
    const esp_timer_create_args_t ignite_timer_args = {
        .callback = &ignite_timer_callback,
        .arg = (void *)arg,
        .name = "ignite"};
    const esp_timer_create_args_t preignite_notification_timer_args = {
        .callback = &preignite_notification_callback,
        .arg = (void *)arg,
        .name = "preignite-notification"};

    ESP_ERROR_CHECK(esp_timer_create(&ignite_timer_args, &ignite_timer));
    ESP_ERROR_CHECK(esp_timer_create(&preignite_notification_timer_args, &preignite_notification_timer));
    ESP_LOGI(TAG, "Setup timers complete");
}

static void preignite_notification_callback(void *arg)
{
    igniter_args_handle params = (igniter_args_handle)arg;
    ESP_LOGI(TAG, "PRE-IGNITE NOTIFICATION");
    gpio_set_level(params->ingniter_notification_pin, params->info_blink_direct);
}

static void ignite_timer_callback(void *arg)
{
    igniter_args_handle params = (igniter_args_handle)arg;
    ESP_LOGI(TAG, "IGNITE");
    gpio_set_level(params->igniter_pin, true);
    vTaskDelay(100 / portTICK_PERIOD_MS);
    gpio_set_level(params->igniter_pin, false);
    gpio_set_level(params->ingniter_notification_pin, !params->info_blink_direct);
    ESP_LOGI(TAG, "IGNITE COMPLETE");
}
#pragma endregion