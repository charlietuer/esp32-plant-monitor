/**
 * @file led.cpp
 * @brief LED handling for the ESP32 plant monitor project.
 */

#include "led.hpp"
#include "driver/gpio.h"

/**
 * @brief Construct a new LED object with specified GPIO pin and active-low configuration.
 */
LED::LED(gpio_num_t pin, bool active_low)
    : pin(pin), active_low(active_low), state(false) {}

/**
 * @brief Initialize the LED GPIO pin as an output and set it to the off state.
 */
void LED::init() {
    gpio_reset_pin(pin);
    gpio_set_direction(pin, GPIO_MODE_OUTPUT);
    set(false); // Start off
}

/**
 * @brief Set the LED state to on or off, taking into account active-low configuration.
 */
void LED::set(bool on) {
    state = on;
    gpio_set_level(pin, active_low ? !on : on);
}

/**
 * @brief Toggle the LED state from on to off or vice versa.
 */
void LED::toggle() {
    set(!state);
}

/**
 * @brief Get the current state of the LED (true for on, false for off).
 */
bool LED::get() const {
    return state;
}