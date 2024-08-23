#include "ADC.h"  // Include the header file for ADC functionality

// Function to continuously read ADC values
void adc_read(void *pvParameters) {
    
    // Configure ADC1 settings
    ESP_ERROR_CHECK(adc1_config_width(ADC_WIDTH_BIT_12));  // Set the ADC width to 12 bits 
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11)); // Set the attenuation for ADC1_CHANNEL_0 to 11 dB for better range

    while (1) {  
        // Read ADC value
        int adc_value = adc1_get_raw(ADC_CHANNEL);

        adc_delay = adc_value;  // Update the global adc_delay variable with the latest ADC reading
        vTaskDelay(pdMS_TO_TICKS(50));  // Delay for 50 milliseconds before the next reading
    }
}