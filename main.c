#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define BUTTON_PIN GPIO_NUM_2
#define LED_PIN GPIO_NUM_13
#define COUNTDOWN_TIME 10  // Countdown time in seconds

void countdown_task(void *pvParameter);  // Function prototype

void button_task(void *pvParameter) {
    while (1) {
        if (gpio_get_level(BUTTON_PIN) == 0) {
            vTaskDelay(50 / portTICK_PERIOD_MS);  // Debounce the button
            if (gpio_get_level(BUTTON_PIN) == 0) {
                xTaskCreate(countdown_task, "countdown_task", 2048, NULL, 10, NULL);
            }
        }
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}

void countdown_task(void *pvParameter) {
    int countdownRemaining = COUNTDOWN_TIME;
    while (countdownRemaining > 0) {
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(500 / portTICK_PERIOD_MS);  // LED on for 0.5 seconds
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(500 / portTICK_PERIOD_MS);  // LED off for 0.5 seconds
        countdownRemaining--;
    }
    vTaskDelete(NULL);
}

void app_main() {
    gpio_pad_select_gpio(BUTTON_PIN);
    gpio_set_direction(BUTTON_PIN, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_PIN, GPIO_PULLUP_ONLY);

    gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_PIN, 0);

    xTaskCreate(button_task, "button_task", 2048, NULL, 10, NULL);
}
