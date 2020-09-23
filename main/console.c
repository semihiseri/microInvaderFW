#include <stdio.h>
#include <string.h>
#include "esp_system.h"
#include "esp_log.h"
#include "esp_console.h"
#include "esp_vfs_dev.h"
#include "esp_wifi.h"
#include "esp_vfs_fat.h"
#include "driver/uart.h"
#include "linenoise/linenoise.h"
#include "argtable3/argtable3.h"
#include "lwip/err.h"
#include "lwip/sys.h"

char get_user_input_character(char *prompt, bool is_silent) {
    printf(prompt);
    char user_input = 255;
    while (user_input == 255) {
        user_input = fgetc(stdin);
        vTaskDelay(10/portTICK_RATE_MS);
    }
    if (!is_silent) {
        printf("%c\r\n", user_input);
    }
    return user_input;
}

char* get_user_input_string(char *prompt) {
    char* line = NULL;
    while(line == NULL) {
        line = linenoise(prompt);
        vTaskDelay(10/portTICK_RATE_MS);
    }
    return line;
}

void show_help()
{
    printf("robot > no mercy for the helpless \n"
        "robot > just follow the instructions \n"
        "robot > press y to continue \n");

    /*char sad_messages[] [64] = {
        "let's pretend it didn't happen",
        "here, another chance for you",
        "we can forget this", 
        "how difficult can it be?", 
        "humans make mistakes",
        "let's try again",
        "hopeless... go again",
    };*/
}

void show_ip(char* ip_address)
{
    /* For some reason this does not seem to work:
    esp_netif_ip_info_t ip_info;
    if (esp_netif_get_ip_info(ESP_IF_WIFI_STA, &ip_info) == ESP_OK) {
        printf("IP address: " IPSTR "\n", IP2STR(&ip_info.ip));
    } else {
        printf("Could not get IP address.\n");
    }
    As a workaround the ip address is passed along to this task
    */
    printf("robot > IP address: %s\n", ip_address);
}

void configure_wifi()
{
    char* ssid = get_user_input_string("WiFi SSID: ");
    printf("\n");
    char* password = get_user_input_string("WiFi password: ");

    wifi_config_t wifi_config = {
        .sta = {
            .ssid = "",
            .password = "",
        .threshold.authmode = WIFI_AUTH_WPA2_PSK,
            .pmf_cfg = {
                .capable = true,
                .required = false
            },
        },
    };
    strcpy((char*)wifi_config.sta.ssid, ssid);
    strcpy((char*)wifi_config.sta.password, password);
    if (esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config) == ESP_OK) {
        printf("The configration has been saved.\n");
    } else {
        printf("The configration could not be saved.\n");
    }

    linenoiseFree(ssid);
    linenoiseFree(password);

    get_user_input_character("Presse any key to reboot.", false);
    esp_restart();
}

void initialize_console()
{
    fflush(stdout);
    fsync(fileno(stdout));
    setvbuf(stdin, NULL, _IONBF, 0);

    /* Minicom, screen, idf_monitor send CR when ENTER key is pressed */
    esp_vfs_dev_uart_set_rx_line_endings(ESP_LINE_ENDINGS_CR);
    /* Move the caret to the beginning of the next line on '\n' */
    esp_vfs_dev_uart_set_tx_line_endings(ESP_LINE_ENDINGS_CRLF);

    const uart_config_t uart_config = {
            .baud_rate = CONFIG_ESP_CONSOLE_UART_BAUDRATE,
            .data_bits = UART_DATA_8_BITS,
            .parity = UART_PARITY_DISABLE,
            .stop_bits = UART_STOP_BITS_1,
            .source_clk = UART_SCLK_REF_TICK,
    };

    ESP_ERROR_CHECK( uart_driver_install(CONFIG_ESP_CONSOLE_UART_NUM, 256, 0, 0, NULL, 0) );
    ESP_ERROR_CHECK( uart_param_config(CONFIG_ESP_CONSOLE_UART_NUM, &uart_config) );
    esp_vfs_dev_uart_use_driver(CONFIG_ESP_CONSOLE_UART_NUM);

    esp_console_config_t console_config = {
            .max_cmdline_args = 8,
            .max_cmdline_length = 256,
    };
    ESP_ERROR_CHECK( esp_console_init(&console_config) );

    linenoiseSetMultiLine(1);
    /*linenoiseSetCompletionCallback(&esp_console_get_completion);
    linenoiseSetHintsCallback((linenoiseHintsCallback*) &esp_console_get_hint);
    linenoiseHistorySetMaxLen(100);*/
}

void console_task(char *ip_address)
{
    initialize_console();

    // wait half a second for the other tasks to stop outputting init messages
    vTaskDelay(500/portTICK_RATE_MS);

    // ensure the user sees the welcome screen by waiting for key press
    get_user_input_character("", true);

    int probe_status = linenoiseProbe();
    if (probe_status) {
        printf("\n"
               "Your terminal application does not support escape sequences.\n"
               "Line editing and history features are disabled.\n"
               "On Windows, try using Putty instead.\n");
        linenoiseSetDumbMode(1);
    }

    printf("\n"
           "robot > ***** MicroInvaderBot Terminal ***** \n"
           "robot > ==================================== \n"
           "robot > h: help \n"
           "robot > i: show IP address \n"
           "robot > w: configure Wi-Fi \n"
           "robot > b: configure Bluetooth \n");

    char selection;
    while(true)
    {
        selection = get_user_input_character(" user > ", false);

        switch(selection)
        {
            case 'h':
                show_help();
                break;
            case 'i':
                show_ip(ip_address);
                break;
            case 'w':
                configure_wifi();
                break;
            case 'b':
                printf("robot > Unfortunately Bluetooth configuration has not been implemented yet.\n");
                break;
        }
    }
}
