/* LED Blinking Example

*/
#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "sdkconfig.h"

#include "LED.h"
#include "ADC.h"

void app_main(void)
{
    xTaskCreate(led_flasher, "led_flasher", 4096, NULL, 1, NULL);
    xTaskCreate(adc_read, "adc_read", 4096, NULL, 1, NULL);
}