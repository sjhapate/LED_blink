#ifndef LED_H
#define LED_H

#define MIN_BLINK_SPEED 0
#define MAX_BLINK_SPEED 50
#define ADC_SCALE_FACTOR 80

#include <string.h>               // Include string manipulation functions
#include "driver/gpio.h"          // Include GPIO driver for GPIO control
#include "esp_log.h"              // Include ESP-IDF logging library
#include "sdkconfig.h"            // Include SDK configuration header
#include "esp_adc_cal.h"          // Include ADC calibration functions
#include "freertos/FreeRTOS.h"    // Include FreeRTOS definitions

#include "driver/rmt_tx.h"        // Include RMT driver for transmitting data to LED strips
#include "led_strip_encoder.h"    // Include LED strip encoder for encoding data to send to LED strips

// Define constants for LED strip configuration
#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      17         // GPIO pin number for the LED strip

// Function prototype for the LED flashing task
void led_flasher(void *pvParameters);

// Declare an external variable to hold the ADC delay value
extern int adc_delay;

#endif // LED_H