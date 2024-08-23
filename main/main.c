/* LED Blinking Example
 * This example demonstrates how to use FreeRTOS tasks to control an LED 
 * by reading the potentiometer ADC values concurrently
 */

#include <stdio.h>                 // Include standard I/O library for printf and related functions
#include <string.h>                // Include string manipulation functions
#include "freertos/FreeRTOS.h"     // Include FreeRTOS definitions
#include "freertos/task.h"         // Include FreeRTOS task definitions
#include "sdkconfig.h"             // Include SDK configuration header

#include "LED.h"                   // Include the LED header file for LED control functions
#include "ADC.h"                   // Include the ADC header file for ADC reading functions

// Main application entry point
void app_main(void)
{
    // Create a FreeRTOS task for LED flashing
    xTaskCreate(led_flasher, "led_flasher", 4096, NULL, 1, NULL);
    // Create a FreeRTOS task for reading ADC values
    xTaskCreate(adc_read, "adc_read", 4096, NULL, 1, NULL);
}