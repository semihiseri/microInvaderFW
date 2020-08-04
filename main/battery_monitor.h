

// Pin definitions
#define BATTERY_MONITOR_CHANNEL 4

extern float battery_status;

void battery_monitor_setup();
void battery_monitor_task(void *args);
