#include "battery_monitor.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/adc.h"
#include "esp_adc_cal.h"

#include "stdio.h"


float battery_status = 0;

esp_adc_cal_characteristics_t adc_characteristic;

void battery_monitor_setup()
{
    printf("LED GPIO init\n");

    adc2_config_channel_atten(BATTERY_MONITOR_CHANNEL, ADC_ATTEN_11db );
    esp_adc_cal_characterize(ADC_UNIT_2, ADC_ATTEN_DB_11, ADC_WIDTH_BIT_12, 1100, &adc_characteristic);
}

/*
 * This function will report battery voltage. The reported value will be exponentially averaged
 */
void battery_monitor_task(void *args)
{
    int raw_adc;
    float v_batt = 0;
    
    while (1)
    {
        // TODO: protect battery_state variables with mutexes
        adc2_get_raw(BATTERY_MONITOR_CHANNEL, ADC_WIDTH_BIT_12, &raw_adc);
        raw_adc = esp_adc_cal_raw_to_voltage(raw_adc, &adc_characteristic);
        
        v_batt = ((float) raw_adc)*(3.2/1000); // The voltage divider divides by 3.2 and raw_adc is in mV
        
        if (battery_status == 0)
        {
            battery_status = v_batt;
        }
        else
        {
            battery_status = 0.9*battery_status + 0.1*v_batt;
        }
                
        vTaskDelay(100/portTICK_RATE_MS);
    }
}
