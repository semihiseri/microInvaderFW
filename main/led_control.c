#include "led_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"

#include "stdio.h"


char wifi_led_state = 0;
char server_led_state = 0;
char activity_led_state = 0;

void led_control_setup()
{
    printf("LED GPIO init\n");
    gpio_config_t io_conf;
    
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = LED_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    
    gpio_config(&io_conf);
}

/*
 * Status LEDs will turn themselves off if they aren't kept alive. This is the default behavior
 */
void led_control_task(void *args)
{
    while (1)
    {
        // TODO: protect *_led_state variables with mutexes
        
        if (wifi_led_state)
        {
            gpio_set_level(LED_WIFI, 1);
            wifi_led_state = 0;
        }
        else
        {
            gpio_set_level(LED_WIFI, 0);
        }
        
        if (server_led_state)
        {
            gpio_set_level(LED_SERVER, 1);
            server_led_state = 0;
        }
        else
        {
            gpio_set_level(LED_SERVER, 0);
        }
        
        if (activity_led_state)
        {
            gpio_set_level(LED_ACTIVITY, 1);
            activity_led_state = 0;
        }
        else
        {
            gpio_set_level(LED_ACTIVITY, 0);
        }
                
        vTaskDelay(200/portTICK_RATE_MS);
    }
}
