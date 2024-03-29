#include "battery_task.h"

static const char *TAG = "battery";
void battery_init(void)
{
    adc1_config_width(ADC_WIDTH_BIT_9);//0--511;
    adc1_config_channel_atten(ADC_CHANNEL_5,ADC_ATTEN_DB_11);
}

uint8_t battery_get(void)
{
    ESP_LOGI(TAG, "begin");
    static int adc_value = 0;
    adc_value = adc1_get_raw(ADC_CHANNEL_5);
    ESP_LOGI(TAG, "%d",adc_value);
    return 0;
}