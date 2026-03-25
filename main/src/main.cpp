#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"
#include "hal.hpp"

static const char *TAG = "BUTTON";
static QueueHandle_t button_evt_queue;
static bool led_state = false;

#define PUMP_ON 0
#define PUMP_OFF 1

// class Plant {
// private:
//     Pump pump;
//     MoistureSensor sensor;
//     LED status_led;
//     Display display;
//     SettingDial dial;
    
// public:
//     Plant(uint8_t plant_id, gpio_num_t pump_pin, 
//           adc_channel_t sensor_channel, gpio_num_t led_pin, /* ... */);
    
//     void update();  // Read sensor, check dial, update display
//     void water();
//     void displayStats();
// };

void IRAM_ATTR button_isr_handler(void *)
{
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    uint32_t val = 1; // dummy event value
    xQueueSendFromISR(button_evt_queue, &val, &xHigherPriorityTaskWoken);
    if (xHigherPriorityTaskWoken) portYIELD_FROM_ISR();
}

void button_task(void *)
{
    uint32_t evt;
    for (;;) {
        if (xQueueReceive(button_evt_queue, &evt, portMAX_DELAY)) {

            // Debounce delay
            vTaskDelay(pdMS_TO_TICKS(30));

            // Confirm button still pressed (active-low)
            if (gpio_get_level(BUTTON_GPIO) == 0) {
                led_state = !led_state;
                gpio_set_level(LED_GPIO, led_state);
                ESP_LOGI(TAG, "LED is now %s", led_state ? "ON!" : "OFF!");
                gpio_set_level(PUMP_GPIO, led_state ? PUMP_ON : PUMP_OFF);
                ESP_LOGI(TAG, "PUMP is now %s", led_state ? "ON!" : "OFF!");

                // Wait for button release to avoid retriggering
                while (gpio_get_level(BUTTON_GPIO) == 0) {
                    vTaskDelay(pdMS_TO_TICKS(10));
                }
            }
        }
    }
}

void gpio_init(void) {
    // Configure LED
    gpio_reset_pin(LED_GPIO);
    gpio_set_direction(LED_GPIO, GPIO_MODE_OUTPUT);
    gpio_set_level(LED_GPIO, 0);

    // Configure Button
    gpio_reset_pin(BUTTON_GPIO);
    gpio_set_direction(BUTTON_GPIO, GPIO_MODE_INPUT);
    gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_intr_type(BUTTON_GPIO, GPIO_INTR_NEGEDGE);

    // Configure Pump
    gpio_reset_pin(PUMP_GPIO);
    gpio_set_direction(PUMP_GPIO, GPIO_MODE_OUTPUT);
    //gpio_set_pull_mode(BUTTON_GPIO, GPIO_PULLUP_ONLY);
    gpio_set_level(PUMP_GPIO, PUMP_OFF);
}

extern "C" void app_main()
{
    gpio_init();

    // Create event queue for uint32_t events
    button_evt_queue = xQueueCreate(10, sizeof(uint32_t));

    // Install and register ISR
    gpio_install_isr_service(0);
    gpio_isr_handler_add(BUTTON_GPIO, button_isr_handler, nullptr);

    // Create button task
    xTaskCreate(button_task, "button_task", 2048, nullptr, 10, nullptr);
}
