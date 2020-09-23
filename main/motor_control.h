

// Pin definitions
#define MOTOR_1_DIR_A   21
#define MOTOR_1_DIR_B   23
#define MOTOR_1_PWM     19

#define MOTOR_2_DIR_A   33
#define MOTOR_2_DIR_B   25
#define MOTOR_2_PWM     32

#define MOTOR_DIR_PIN_SEL ((1ULL<<MOTOR_1_DIR_A) | (1ULL<<MOTOR_1_DIR_B) | (1ULL<<MOTOR_2_DIR_A) | (1ULL<<MOTOR_2_DIR_B))

extern float motor_1_pwm;
extern float motor_2_pwm;

void motor_control_set_pwm_values(float motor_1, float motor_2);
void motor_control_setup();
void motor_control_task(void *args);
