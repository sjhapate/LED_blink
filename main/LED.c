#include "LED.h" // Include the LED header file for LED control functions

static const char *TAG = "LED"; // Log tag for this module

uint32_t AdcValue = 3000;                            // Variable to store the ADC delay value (initially set to 3000)
static uint8_t led_strip_pixels[3] = {255, 255, 255}; // Array to hold RGB values for the LED (initialized to white)

// Function to Change the color of the LED depending on the frequency
static void set_led_col(uint8_t *value)
{

    if (*value != 1)
    {
        float freq = (float)AdcValue / ADC_MAX_VALUE * 10.0; 
        uint8_t red = 0, green = 0, blue = 0;

        // ESP_LOGI(TAG, "freq : %f", freq);  // Log the current delay value
        // Map frequency to RGB colors
        red = (uint8_t)(255 * (1 - freq / 10)); // Red decreases with frequency
        green = (uint8_t)(255 * (freq / 10));   // Green increases with frequency
        blue = (uint8_t)(255 * (freq / 10));    // Blue increases with frequency

        led_strip_pixels[0] = red;
        led_strip_pixels[1] = green;
        led_strip_pixels[2] = blue;
    }
    else
    {
        // If blink speed is zero, set LED to white
        led_strip_pixels[0] = 255; // Red
        led_strip_pixels[1] = 255; // Green
        led_strip_pixels[2] = 255; // Blue
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
        uint8_t Task = 2;
        int32_t delay;

        delay = (MAX_BLINK_SPEED - ((RANGE_DIFFRENCE * AdcValue) / ADC_MAX_VALUE)); // Calculate blink speed based on ADC dela

        // Handle LED blinking or staying on
        if ((delay > 0) && (delay < MAX_BLINK_SPEED-1))
        {
            if (loop == 0)
            {
                set_led_col(&Task); // Set LED color
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