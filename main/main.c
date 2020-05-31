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

#include "motor_control.h"



void app_main(void)
{
    float x;
    motor_control_setup();
    
    xTaskCreate(motor_control_task, "motor_control", 4096, NULL, 5, NULL);
    
    while (1)
    {
        for (x=-100; x<100; x++)
        {
            motor_1_pwm = x;
            motor_2_pwm = x;
            vTaskDelay(10/portTICK_RATE_MS);
        }
    }
}
