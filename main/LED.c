#include "LED.h"

static const char *TAG = "LED";

int adc_delay=3000;
static uint8_t led_strip_pixels[3]={255,255,255}; // Multiplied by 3 as it will hold r,g,b value


static void set_led_col()
{
    int blinkSpeed=(50-(adc_delay/80))/10;
        switch (blinkSpeed)
        {
        case 0:
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 255;
            break;
        case 1:
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 0;
                led_strip_pixels[2] = 0;

            break;
        case 2:
                led_strip_pixels[0] = 0;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 0;

            break;
        case 3:
                led_strip_pixels[0] = 0;
                led_strip_pixels[1] = 0;
                led_strip_pixels[2] = 255;

            break;
        case 4:
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 255;
            break;
        case 5:
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 0;
            break;
        default:
            break;
        }
}

void led_flasher()
{
    uint8_t loop=0;
    ESP_LOGI(TAG, "Create RMT TX channel");
    rmt_channel_handle_t led_chan = NULL;
    rmt_tx_channel_config_t tx_chan_config = {
        .clk_src = RMT_CLK_SRC_DEFAULT, // select source clock
        .gpio_num = RMT_LED_STRIP_GPIO_NUM,
        .mem_block_symbols = 64, // increase the block size can make the LED less flickering
        .resolution_hz = RMT_LED_STRIP_RESOLUTION_HZ,
        .trans_queue_depth = 4, // set the number of transactions that can be pending in the background
    };
    ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_chan_config, &led_chan));

    ESP_LOGI(TAG, "Install led strip encoder");
    rmt_encoder_handle_t led_encoder = NULL;
    led_strip_encoder_config_t encoder_config = {
        .resolution = RMT_LED_STRIP_RESOLUTION_HZ,
    };
    ESP_ERROR_CHECK(rmt_new_led_strip_encoder(&encoder_config, &led_encoder));

    ESP_LOGI(TAG, "Enable RMT TX channel");
    ESP_ERROR_CHECK(rmt_enable(led_chan));

    ESP_LOGI(TAG, "Start LED rainbow chase");
    rmt_transmit_config_t tx_config = {
        .loop_count = 0, // no transfer loop
    };

    while (1)
    {
        int blinkSpeed=50-(adc_delay/80);
        if(blinkSpeed>0)
        {
            if(loop==0)
            {
            set_led_col();
            loop=1;
            } else {
                led_strip_pixels[0] = 0;
                led_strip_pixels[1] = 0;
                led_strip_pixels[2] = 0;
                loop=0;
            }
        } else {
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 255;
        }
        /*
        switch (loop)
        {
        case 0:
                led_strip_pixels[0] = 0;
                led_strip_pixels[1] = 0;
                led_strip_pixels[2] = 0;
                loop=4;
            break;
        case 1:
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 0;
                led_strip_pixels[2] = 0;
                loop++;
            break;
        case 2:
                led_strip_pixels[0] = 0;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 0;
                loop++;
            break;
        case 3:
                led_strip_pixels[0] = 0;
                led_strip_pixels[1] = 0;
                led_strip_pixels[2] = 255;
                loop++;
            break;
        case 4:
                led_strip_pixels[0] = 255;
                led_strip_pixels[1] = 255;
                led_strip_pixels[2] = 255;
                loop=0;
            break;
        default:
            break;
        }
        */

            // Flush RGB values to LEDs
        ESP_ERROR_CHECK(rmt_transmit(led_chan, led_encoder, led_strip_pixels, sizeof(led_strip_pixels), &tx_config));
        ESP_ERROR_CHECK(rmt_tx_wait_all_done(led_chan, portMAX_DELAY));
        
        
        ESP_LOGI(TAG, "Delay : %d", blinkSpeed);
        vTaskDelay(blinkSpeed / portTICK_PERIOD_MS);
    }
}




