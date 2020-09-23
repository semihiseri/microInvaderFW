/* GPIO Example

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <lwip/netdb.h>

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "esp_wifi.h"
#include "esp_event.h"

#include "battery_monitor.h"
#include "led_control.h"
#include "motor_control.h"
#include "bluetooth_spp.h"
#include "wifi.h"
#include "udp_server.h"
#include "console.h"

//#define TEST_MODE
#define BLUETOOTH
//#define WIFI

/*
 * This does stuff so that we can see everything works nicely
 */
void device_test(void)
{
    int x;
    
    motor_1_pwm = 100;
    motor_2_pwm = -100;
    
    while (1)
    {
        break;
        for (x=-100; x<100; x++)
        {
            motor_1_pwm = x;
            motor_2_pwm = x;
            
            wifi_led_state = x > 32;
            server_led_state = x > 65;
            activity_led_state = x > 98;
            
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
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    void *ip_address = connect_wifi();

    // Subcomponent setup
    motor_control_setup();
    led_control_setup();
    battery_monitor_setup();

    // Start subcomponent tasks
    xTaskCreate(motor_control_task, "motor_control", 4096, NULL, 5, NULL);
    xTaskCreate(led_control_task, "led_control", 4096, NULL, 5, NULL);
    xTaskCreate(battery_monitor_task, "battery_monitor", 4096, NULL, 5, NULL);
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
    xTaskCreate(console_task, "console", 4096, ip_address, 5, NULL);
    
#ifdef TEST_MODE
    device_test();
#elif defined WIFI
    //console_task();
    /*while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);
    }*/
#elif defined BLUETOOTH
    setup_bt();
#endif

}
