#include "LED.h"  // Include the LED header file for LED control functions

static const char *TAG = "LED";  // Log tag for this module

int adc_delay = 3000;  // Variable to store the ADC delay value (initially set to 3000)
static uint8_t led_strip_pixels[3] = {255, 255, 255}; // Array to hold RGB values for the LED (initialized to white)

// Function to set the LED color based on blink speed
static void set_led_col(int blinkSpeed ) {
    // Calculate blink speed based on ADC delay
    blinkSpeed = (50 - (adc_delay / 80)) / 10;  // Normalize blink speed

    // Set LED color based on calculated blink speed
    switch (blinkSpeed) {
        case 0:  // If blinkSpeed is 0, set to white
            led_strip_pixels[0] = 255;  // Red
            led_strip_pixels[1] = 255;  // Green
            led_strip_pixels[2] = 255;  // Blue
            break;
        case 1:  // Set to red
            led_strip_pixels[0] = 255;  
            led_strip_pixels[1] = 0;    
            led_strip_pixels[2] = 0;    
            break;
        case 2:  // Set to green
            led_strip_pixels[0] = 0;    
            led_strip_pixels[1] = 255;  
            led_strip_pixels[2] = 0;    
            break;
        case 3:  // Set to blue
            led_strip_pixels[0] = 0;    
            led_strip_pixels[1] = 0;    
            led_strip_pixels[2] = 255;  
            break;
        case 4:  // Set to white
            led_strip_pixels[0] = 255;  
            led_strip_pixels[1] = 255;  
            led_strip_pixels[2] = 255;  
            break;
        case 5:  // Set to yellow
            led_strip_pixels[0] = 255;  
            led_strip_pixels[1] = 255;  
            led_strip_pixels[2] = 0;    
            break;
        default:
            break;  // Do nothing for other values
    }
}

// Function to flash the LED
void led_flasher() {
    uint8_t loop = 0;  // Loop control variable
    ESP_LOGI(TAG, "Create RMT TX channel");  // Log message indicating the creation of RMT TX channel

    // RMT TX channel configuration
    rmt_channel_handle_t led_chan = NULL;  // Handle for RMT channel
    
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // Select source clock
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,  // Set GPIO pin for LED strip
        .mem_block_symbols = 64,  // Increase the block size to reduce flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,  // Set resolution
        .trans_queue_depth = 4,  // Set the number of transactions that can be pending
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));  // Create new RMT TX channel

    ESP_LOGI(TAG, "Install led strip encoder");  // Log message indicating installation of LED strip encoder
    rmt_encoder_handle_t led_encoder = NULL;  // Handle for RMT encoder
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,  // Set resolution for encoder
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));  // Create new LED strip encoder

    ESP_LOGI(TAG, "Enable RMT TX channel");  // Log message indicating enabling of RMT TX channel
    ESP_ERROR_CHECK(rmt_enable(led_chan));  // Enable the RMT TX channel

    ESP_LOGI(TAG, "Start LED rainbow chase");  // Log message indicating the start of LED control
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // No transfer loop
    };

    while (1) {
        int blinkSpeed = MAX_BLINK_SPEED - (adc_delay / ADC_SCALE_FACTOR);  // Calculate blink speed based on ADC delay

        if (blinkSpeed > MIN_BLINK_SPEED) {
            if (loop == 0) {
                set_led_col(blinkSpeed);  // Set LED color based on blink speed
                loop = 1;  // Update loop variable
            } else {
                // Turn off the LED
                led_strip_pixels[0] = 0;  // Red
                led_strip_pixels[1] = 0;  // Green
                led_strip_pixels[2] = 0;  // Blue
                loop = 0;  // Reset loop variable
            }
        } else {
            // If blink speed is zero, set LED to white
            led_strip_pixels[0] = 255;  // Red
            led_strip_pixels[1] = 255;  // Green
            led_strip_pixels[2] = 255;  // Blue
        }
             
        // Flush RGB values to LEDs
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));  // Wait until all transmissions are done

        ESP_LOGI(TAG, "Delay : %d", blinkSpeed);  // Log the current delay value
        vTaskDelay(blinkSpeed / portTICK_PERIOD_MS);  // Delay for the calculated blink speed
    }
}