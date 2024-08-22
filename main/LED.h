#ifndef LED_H
#define LED_H

#include <string.h>
#include "driver/gpio.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "esp_adc_cal.h"
#include "freertos/FreeRTOS.h"

#include "driver/rmt_tx.h"
#include "led_strip_encoder.h"

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      17

void led_flasher();

extern int adc_delay;

#define RMT_LED_STRIP_RESOLUTION_HZ 10000000 // 10MHz resolution, 1 tick = 0.1us (led strip needs a high resolution)
#define RMT_LED_STRIP_GPIO_NUM      17

#endif // LED_H
