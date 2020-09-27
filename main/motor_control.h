

// Pin definitions
#define RIGHT_MOTOR_DIR_REVERSE 21
#define RIGHT_MOTOR_DIR_FORWARD 23
#define RIGHT_MOTOR_PWM         19

#define LEFT_MOTOR_DIR_REVERSE  25
#define LEFT_MOTOR_DIR_FORWARD  33
#define LEFT_MOTOR_PWM          32

#define MOTOR_DIR_PIN_SEL ((1ULL<<RIGHT_MOTOR_DIR_REVERSE) | (1ULL<<RIGHT_MOTOR_DIR_FORWARD) | (1ULL<<LEFT_MOTOR_DIR_REVERSE) | (1ULL<<LEFT_MOTOR_DIR_FORWARD))

extern float motor_value_right;
extern float motor_value_left;

void motor_control_set_values(float motor_left, float motor_right);
void motor_control_setup();
void motor_control_task(void *args);
