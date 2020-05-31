#include "motor_control.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/gpio.h"
#include "driver/mcpwm.h" 

#include "math.h"
#include "stdio.h"


float motor_1_pwm = 0;
float motor_2_pwm = 0;

void gpio_init()
{
    printf("Motor control GPIO init\n");
    gpio_config_t io_conf;
    
    io_conf.intr_type = GPIO_PIN_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = MOTOR_DIR_PIN_SEL;
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    
    gpio_config(&io_conf);
}

void pwm_gpio_config()
{
    printf("MCPWM GPIO init\n");
    mcpwm_pin_config_t pin_config = {
        .mcpwm0a_out_num = MOTOR_1_PWM,
        .mcpwm0b_out_num = MOTOR_2_PWM,
    };
    
    mcpwm_set_pin(MCPWM_UNIT_0, &pin_config);
}

void mcpwm_config()
{
    printf("MCPWM init\n");
    mcpwm_config_t pwm_config;
    
    pwm_config.frequency = 1000;    //frequency = 1000Hz
    pwm_config.cmpr_a = 0.0;       // initial duty cycle 0
    pwm_config.cmpr_b = 0.0;       // initial duty cycle 0
    pwm_config.counter_mode = MCPWM_UP_COUNTER;
    pwm_config.duty_mode = MCPWM_DUTY_MODE_0;
    
    mcpwm_init(MCPWM_UNIT_0, MCPWM_TIMER_0, &pwm_config); 
}

void motor_control_setup()
{
    gpio_init();
    pwm_gpio_config();
    mcpwm_config();
}

void motor_control_task(void *args)
{
    while (1)
    {
        // TODO: protect motor_x_pwm variables with mutexes
        if (fabs(motor_1_pwm) > 100 || fabs(motor_2_pwm) > 100)
        {
            printf("ERROR: Motor duty cycle cannot exceed 100 \n"); // TODO: make this a proper error log
            motor_1_pwm = 0;
            motor_2_pwm = 0;
        }
        
        if (motor_1_pwm < 0)
        {
            gpio_set_level(MOTOR_1_DIR_A, 1);
            gpio_set_level(MOTOR_1_DIR_B, 0);
        }
        else
        {
            gpio_set_level(MOTOR_1_DIR_A, 0);
            gpio_set_level(MOTOR_1_DIR_B, 1);
        }
        
        if (motor_2_pwm < 0)
        {
            gpio_set_level(MOTOR_2_DIR_A, 1);
            gpio_set_level(MOTOR_2_DIR_B, 0);
        }
        else
        {
            gpio_set_level(MOTOR_2_DIR_A, 0);
            gpio_set_level(MOTOR_2_DIR_B, 1);
        }
        
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_A, fabs(motor_1_pwm));
        mcpwm_set_duty(MCPWM_UNIT_0, MCPWM_TIMER_0, MCPWM_OPR_B, fabs(motor_2_pwm));
        
        vTaskDelay(10/portTICK_RATE_MS);
    }
}
