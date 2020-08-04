/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "battery_monitor.h"
#include "led_control.h"
#include "motor_control.h"

#define TEST_MODE

/*
 * This does stuff so that we can see everything works nicely
 */
void device_test(void)
{
    int x;
    
    while (1)
    {
        for (x=-100; x<100; x++)
        {
            motor_1_pwm = x;
            motor_2_pwm = x;
            
            if (x == 33)
            {
                wifi_led_state = 1;
            }
            
            if (x == 66)
            {
                server_led_state = 1;
            }
            
            if (x == 99)
            {
                activity_led_state = 1;
            }
            
            if (!(x%10))
            {
                printf("Battery voltage: %f\n", battery_status);
            }
            
            vTaskDelay(10/portTICK_RATE_MS);
        }
    }
}

void app_main(void)
{
    // Subcomponent setup
    motor_control_setup();
    led_control_setup();
    battery_monitor_setup();
    
    // Start subcomponent tasks
    xTaskCreate(motor_control_task, "motor_control", 4096, NULL, 5, NULL);
    xTaskCreate(led_control_task, "led_control", 4096, NULL, 5, NULL);
    xTaskCreate(battery_monitor_task, "battery_monitor", 4096, NULL, 5, NULL);
    
#ifdef TEST_MODE
    device_test();
#else
    while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);
    }
#endif

}
