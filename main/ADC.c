#include "ADC.h"

void adc_read()
{
    static int adc_raw[2][10];
    //ADC1 config
    ESP_ERROR_CHECK(adc1_config_width(0));
    ESP_ERROR_CHECK(adc1_config_channel_atten(ADC1_CHANNEL_0, 3));
    while (1) {
        adc_raw[0][0] = adc1_get_raw(0);
        //ESP_LOGI(TAG, "raw  data: %d", adc_raw[0][0]);
        adc_delay=adc_raw[0][0];
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}