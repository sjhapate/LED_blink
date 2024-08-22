#include "led_control.h"

// Function to set up GPIO
void setup_gpio(void) {
    gpio_reset_pin(BLINK_GPIO); // Reset the GPIO pin
    gpio_set_direction(BLINK_GPIO, GPIO_MODE_OUTPUT); // Set the GPIO pin as output
}

// Function to set up ADC
void setup_adc(void) {
    adc1_config_width(ADC_WIDTH_BIT_12); // Set ADC width to 12 bits
    adc1_config_channel_atten(ADC_CHANNEL, ADC_ATTEN_DB_11); // Set attenuation for the ADC channel
}

// Task to blink the LED based on potentiometer input
void blink_task(void *pvParameter) {
    uint32_t adc_reading = 0; // Variable to store ADC reading
    int delay_ms; // Variable to store calculated delay

    while (1) {
        // Read the potentiometer value
        adc_reading = adc1_get_raw(ADC_CHANNEL);

        // Log an adc_reading
        //ESP_LOGI(TAG, "adc_reading: %lu",adc_reading); 

        // Map the ADC value to a delay time (0 Hz to 10 Hz)
        if (adc_reading == 0) {
            // 0 Hz means the LED is constantly on
            gpio_set_level(BLINK_GPIO, 1); // Turn on the LED
            vTaskDelay(pdMS_TO_TICKS(100)); // Small delay to yield control
        } else {
            // Calculate delay based on frequency
            int frequency = (adc_reading * 10) / 3245; // Map ADC value to frequency (0 to 10 Hz)

            // Log an frequency
            //ESP_LOGI(TAG, "frequency: %d",frequency); 

            if (frequency == 0) {
                frequency = 1; // Set minimum frequency to avoid division by zero
            }

            delay_ms = 1000 / (2 * frequency); // Calculate delay in milliseconds

            // Log an adc_reading
            //ESP_LOGI(TAG, "delay_ms: %d",delay_ms); 

            // Blink the LED at the calculated frequency
            gpio_set_level(BLINK_GPIO, 1); // Turn on the LED
            vTaskDelay(pdMS_TO_TICKS(delay_ms)); // Wait for the delay
            gpio_set_level(BLINK_GPIO, 0); // Turn off the LED
            vTaskDelay(pdMS_TO_TICKS(delay_ms)); // Wait for the delay
        }
    }
}