

// Pin definitions
#define LED_WIFI        17
#define LED_SERVER      5
#define LED_ACTIVITY    18

#define LED_PIN_SEL ((1ULL<<LED_WIFI) | (1ULL<<LED_SERVER) | (1ULL<<LED_ACTIVITY))

extern char wifi_led_state;
extern char server_led_state;
extern char activity_led_state;

void led_control_setup();
void led_control_task(void *args);
