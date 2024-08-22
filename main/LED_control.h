#ifndef LED_CONTROL_H
#define LED_CONTROL_H

#include <stdint.h>  // For uint32_t
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
//#include "esp_log.h"

//static const char *TAG = "ADC_f";      // Define the TAG for logging

// Define GPIO and ADC channel
#define BLINK_GPIO GPIO_NUM_2          // Use the built-in LED on GPIO 2
#define ADC_CHANNEL ADC1_CHANNEL_0     // GPIO 36 for potentiometer input

// Function declarations
void blink_task(void *pvParameter);
void setup_gpio(void);
void setup_adc(void);

#endif // LED_CONTROL_H