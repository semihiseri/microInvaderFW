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

//#define TEST_MODE
#define BLUETOOTH
//#define WIFI

char get_user_input(void)
{
    char user_input;
    
    printf("  user > ");
    
    while(1)
    {
        user_input = fgetc(stdin);
        
        if (user_input != 255)
        {
            break;
        }
        vTaskDelay(10/portTICK_RATE_MS);
    }
    
    return user_input;
}

char get_quiet_input(void)
{
    char user_input;
    
    while(1)
    {
        user_input = fgetc(stdin);
        
        if (user_input != 255)
        {
            break;
        }
        vTaskDelay(10/portTICK_RATE_MS);
    }
    
    return user_input;
}

/*
 * Main task. This is the communication interface between people and the robot
 */
void console_task(void)
{
    char user_input;
    
    int menu_state = 0;
    
    int x;
    
    char ssid[128] = "";
    char pass[128] = "";
    
    
    char main_message[] = "\
 robot > ***** MicroInvaderBot Terminal ***** \n\
 robot > ==================================== \n\
 robot > h: help \n\
 robot > a: add wifi access point \n\
 robot > s: saved access points \n\
";
     
    char help_message[] = "\
 robot > no mercy for the helpless \n\
 robot > just follow the instructions \n\
 robot > press y to continue \n\
";
    
    char sad_messages[] [255] = {
        "let's pretend it didn't happen",
        "here, another chance for you",
        "we can forget this", 
        "how difficult can it be?", 
        "humans make mistakes",
        "let's try again",
        "hopeless... go again",
    };
    
     
    while (1)
    {
        switch (menu_state)
        {
            case 0: // main menu
                printf(main_message);
                
                user_input = get_user_input();
                printf("%c %d\n", user_input, user_input);
                
                if (user_input == 'h')
                    menu_state = 4;
                if (user_input == 'a')
                    menu_state = 1;
                if (user_input == 's')
                    menu_state = 3;
                
                break;
            case 1: // wifi setup menu, ssid
                printf(" robot > enter ssid\n");
                printf("  user > ");
                
                x = 0;
                
                while (1) // loop until some escape character
                {
                    user_input = get_quiet_input();
                    
                    if (user_input == 10 || user_input == 3) // Until user presses enter or ctrl+c (FIXME: is this a good one?
                    {
                        printf("\n");
                        break;
                    }
                    
                    ssid[x] = user_input;
                    x++;
                    printf("%c", user_input);
                }
                
                if (user_input == 3) // if the user cancels
                {
                    printf(" robot > aborted\n");
                    menu_state = 0;
                }
                else
                {
                    ssid[x] = '\0';
                    menu_state = 2; // if nothing bad happens
                }
                
                break;
            case 2: // wifi setup menu, password
                printf(" robot > alright, now enter the password\n");
                printf("  user > ");
                
                x = 0;
                
                while (1) // loop until some escape character
                {
                    user_input = get_quiet_input();
                    
                    if (user_input == 10 || user_input == 3) // Until user presses enter or ctrl+c (FIXME: is this a good one?
                    {
                        printf("\n");

                        /*
                        TODO: Write WIFI config. Perhaps like this:
                        wifi_config_t wifi_config = {
                            .sta = {
                                .ssid = "selected ssid",
                                .password = "my super secret password",
                            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
                                .pmf_cfg = {
                                    .capable = true,
                                    .required = false
                                },
                            },
                        };
                        ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
                        */

                        break;
                    }
                    
                    pass[x] = user_input;
                    x++;
                    printf("%c", user_input);
                }
                
                if (user_input == 3) // if the user cancels
                {
                    printf(" robot > aborted\n");
                    menu_state = 0;
                }
                else
                {
                    pass[x] = '\0';
                    
                    printf(" robot > What I got is %s, %s\n", ssid, pass);
                    
                    menu_state = 0;
                }
                break;
            case 3: // saved access points
                printf(" robot > nothing to see here\n");
                menu_state = 0;
                break;
            case 4: // help menu
                printf(help_message);

                user_input = get_user_input();

                if (user_input == 'y')
                {
                    printf("y\n");
                    printf(" robot > nice\n");
                    menu_state = 0;
                }
                else
                {
                    x = esp_random()%(sizeof(sad_messages)/(255*sizeof(char)));
                    printf("\n robot > %s\n", sad_messages[x]);
                }
                break;
            default:
                menu_state = 0;
                break;
        }

        vTaskDelay(100/portTICK_RATE_MS);
    }
}

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
    ESP_ERROR_CHECK(nvs_flash_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    connect_wifi();

    // Subcomponent setup
    motor_control_setup();
    led_control_setup();
    battery_monitor_setup();
    
    // Start subcomponent tasks
    xTaskCreate(motor_control_task, "motor_control", 4096, NULL, 5, NULL);
    xTaskCreate(led_control_task, "led_control", 4096, NULL, 5, NULL);
    xTaskCreate(battery_monitor_task, "battery_monitor", 4096, NULL, 5, NULL);
    xTaskCreate(udp_server_task, "udp_server", 4096, (void*)AF_INET, 5, NULL);
    
#ifdef TEST_MODE
    device_test();
#elif defined WIFI
    console_task();
    /*while(1)
    {
        vTaskDelay(1000/portTICK_RATE_MS);
    }*/
#elif defined BLUETOOTH
    setup_bt();
#endif

}
