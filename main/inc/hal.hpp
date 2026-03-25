#pragma once

// Define the GPIO pin for the LED (GPIO 2 is common for onboard LEDs)
#define LED_GPIO GPIO_NUM_2
#define PUMP_GPIO GPIO_NUM_4
#define BUTTON_GPIO GPIO_NUM_1

// ADC Channels for moisture sensor and potentiometer
#define SENSOR_ADC_CHANNEL ADC_CHANNEL_0
#define DIAL_ADC_CHANNEL   ADC_CHANNEL_1