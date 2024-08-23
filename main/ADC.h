#ifndef ADC_H
#define ADC_H

#define ADC_CHANNEL ADC1_CHANNEL_0  // Potentiometer channel

#include "LED.h"                // Include the LED header file for potential interactions with LED functionality
#include "driver/adc.h"        // Include the ADC driver header for ADC functionality
#include "esp_adc_cal.h"       // Include the ESP ADC calibration header for ADC calibration functions

// Function prototype for the ADC read function
void adc_read();              // Declaration of the adc_read function that will continuously read ADC values

#endif // ADC_H