#include "LED.h" // Include the LED header file for LED control functions

static const char *TAG = "LED"; // Log tag for this module

uint32_t AdcValue = 3000;                            // Variable to store the ADC delay value (initially set to 3000)
static uint8_t led_strip_pixels[3] = {255, 255, 255}; // Array to hold RGB values for the LED (initialized to white)

static void set_led_col()
{
    uint8_t Task = 1; 
    
    if (Task != 1)
    {
        // Calculate frequency scaled to a 0-10 range
        float freq = ((float)AdcValue / ADC_MAX_VALUE) * 10.0f; 
        uint8_t red = 0, green = 0, blue = 0;

        // Optimize color changes by grouping similar settings
        switch ((int)freq) 
        {
            case 0 ... 1:   // Frequency 0-1
                red = 255; green = 0; blue = 0;  
                break;
            case 2 ... 3:   // Frequency 2-3
                red = 200; green = 55; blue = 0; 
                break;
            case 4 ... 5:   // Frequency 4-5
                red = 150; green = 105; blue = 0; 
                break;
            case 6 ... 7:   // Frequency 6-7
                red = 100; green = 155; blue = 0; 
                break;
            case 8 ... 10:  // Frequency 8-10
                red = 0; green = 255; blue = 0;  
                break;
            default:        // Any unexpected frequency
                red = 0; green = 0; blue = 255;  
                break;
        }

        // Set the LED strip colors
        led_strip_pixels[0] = red;
        led_strip_pixels[1] = green;
        led_strip_pixels[2] = blue;
    }
    else
    {
        // Set LED to white when value is 1
        led_strip_pixels[0] = 255; 
        led_strip_pixels[1] = 255; 
        led_strip_pixels[2] = 255; 
    }
}

// Function to flash the LED
void led_flasher(void *pvParameters)
{
    uint8_t loop = 0;                       // Loop control variable
    ESP_LOGI(TAG, "Create RMT TX channel"); // Log message indicating the creation of RMT TX channel

    // RMT TX channel configuration
    rmt_channel_handle_t led_chan = NULL; // Handle for RMT channel

    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT,               // Select source clock
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,           // Set GPIO pin for LED strip
        .mem_block_symbols = 64,                      // Increase the block size to reduce flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ, // Set resolution
        .trans_queue_depth = 4,                       // Set the number of transactions that can be pending
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan)); // Create new RMT TX channel

    ESP_LOGI(TAG, "Install led strip encoder"); // Log message indicating installation of LED strip encoder
    rmt_encoder_handle_t led_encoder = NULL;    // Handle for RMT encoder
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ, // Set resolution for encoder
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder)); // Create new LED strip encoder

    ESP_LOGI(TAG, "Enable RMT TX channel"); // Log message indicating enabling of RMT TX channel
    ESP_ERROR_CHECK(rmt_enable(led_chan));  // Enable the RMT TX channel

    ESP_LOGI(TAG, "Start LED rainbow chase"); // Log message indicating the start of LED control
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // No transfer loop
    };

    while (1)
    {
        /*   
        * ADC range: 0 to 3248 (12-bit ADC scaled)
        * Delay range: 1000 ms to 100 ms 
        */
        int32_t delay;

        delay = (MAX_BLINK_SPEED - ((RANGE_DIFFRENCE * AdcValue) / ADC_MAX_VALUE)); // Calculate blink speed based on ADC dela

        // Handle LED blinking or staying on
        if ((delay > 0) && (delay < MAX_BLINK_SPEED-1))
        {
            if (loop == 0)
            {
                set_led_col(); // Set LED color
                loop = 1;           // Update loop variable
            }
            else
            {
                // Turn off the LED
                led_strip_pixels[0] = 0; // Red
                led_strip_pixels[1] = 0; // Green
                led_strip_pixels[2] = 0; // Blue
                loop = 0;                // Reset loop variable
            }
        }
        else
        {
            // If freq is 0 Hz, LED should be constantly on (White color)
            led_strip_pixels[0] = 255; // Red
            led_strip_pixels[1] = 255; // Green
            led_strip_pixels[2] = 255; // Blue
            delay = MIN_BLINK_SPEED;
        }

        // Flush RGB values to LEDs
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY)); // Wait until all transmissions are done

        ESP_LOGI(TAG, "Delay : %lu", delay);  // Log the current delay value
        vTaskDelay(pdMS_TO_TICKS(delay));     // Delay for the calculated blink speed
    }
}