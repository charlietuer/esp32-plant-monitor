/**
 * @file led.hpp
 * @brief LED handling for the ESP32 plant monitor project.
 */

#pragma once
#include "driver/gpio.h"
#include <cstdint>

class LED {
public:
    // Constructor to initialize the LED GPIO, explicit to avoid implicit conversions of GPIO types
    explicit LED(gpio_num_t pin, bool active_low = false);
     // Default destructor, no dynamic resources to clean up
    ~LED() = default;

    // Public methods to control the LED
    void init();        // Set up GPIO as output
    void set(bool on);  // Turn on/off
    void toggle();      // Flip current state
    bool get() const;   // Get current state

private:
    gpio_num_t pin;
    bool active_low; // For LEDs that are active-low
    bool state;      // Track current state
};