#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>

#include "core/timer.h"

/* 
 * mcu-freq     = 48 000 000
 * freq         = mcu-freq   / ((prescaler - 1) * (arr  - 1))
 * freq         = 48 000 000 / ((48        - 1) * (1000 - 1))
 * freq         =  1 022
 */

#define PRESCALER       (48)
#define ARR_VALUE       (1000)  // auto-reload-register: value at which the timer resets to 0 


void timer_setup(void)
{
        rcc_periph_clock_enable(RCC_TIM3);

        /* High level timer configuration */
        timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
        /* Setup PWM mode */
        timer_set_oc_mode(TIM3, TIM_OC1, TIM_OCM_PWM1);
        /* Enable PWM output */
        timer_enable_counter(TIM3);
        timer_enable_oc_output(TIM3, TIM_OC1);
        /* Setup frequency and resolution */
        timer_set_prescaler(TIM3, PRESCALER - 1);
        timer_set_period(TIM3, ARR_VALUE - 1);
}

void timer_pwm_set_duty_cicle(float duty_cycle)
{
        /* 
         * duty_cycle = (ccr / arr) * 100
         * duty_cycle / 100 =  ccr / arr
         * ccr = arr * (duty_cycle / 100)
         */
        const float raw_value = (float)ARR_VALUE * (duty_cycle / 100.0f);
        timer_set_oc_value(TIM3, TIM_OC1, (uint32_t)raw_value);
}
