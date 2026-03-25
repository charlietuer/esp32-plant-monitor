/**
 * @file button.hpp
 * @brief Button handling for the ESP32 plant monitor project.
 */

#pragma once
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include <functional>

class Button {
public:
    using Callback = std::function<void()>; // Lambda or function pointer

    // Constructor to initialize the button GPIO and set up the interrupt
    Button(gpio_num_t pin, Callback on_press, gpio_pull_mode_t pull_mode = GPIO_PULLUP_ONLY);
    // Destructor if we need it
    ~Button();

    // Init function to set up GPIO and ISR
    void init();

    // Process queue events (to be called in a FreeRTOS task)
    void processEvents(TickType_t timeout = portMAX_DELAY);

    // ISR handler (to be called from the actual ISR)
    static void IRAM_ATTR gpio_isr(void *arg);

private:
    gpio_num_t pin;
    gpio_pull_mode_t pull_mode;
    Callback on_press;
    QueueHandle_t evt_queue; // For ISR-to-task communication
};