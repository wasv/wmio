#include <stdint.h>

#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void tim3_setup()
{
    /* Enable Timer 3. */
    rcc_periph_clock_enable(RCC_TIM3);

    /* Configure Timer 3 w/ No clock divider, edge triggered, upcounting */
    timer_set_mode(TIM3, TIM_CR1_CKD_CK_INT, TIM_CR1_CMS_EDGE, TIM_CR1_DIR_UP);
    /* Prescale CLOCK_FREQ to (SAMPL_FREQ*SAMPL_RESN) */
    timer_set_prescaler(TIM3, 15);

    /* Configure Timer 3 Output 3 Active when counter < compare value */
    timer_set_oc_mode(TIM3, TIM_OC3, TIM_OCM_PWM1);

    /* Configure Timer 3 Output 3 to be active high */
    timer_set_oc_polarity_high(TIM3, TIM_OC3);

    /* Set 127 tick period. */
    timer_set_period(TIM3, 0x7F);

    /* Start at 10% duty cycle. */
    timer_set_oc_value(TIM3, TIM_OC3, 0);

    /* Enable DMA request on update */
    timer_update_on_overflow(TIM3);
    timer_enable_update_event(TIM3);

    /* Enable Timer */
    timer_enable_counter(TIM3);

    /* Enable GPIOB clock. */
    rcc_periph_clock_enable(RCC_GPIOB);

    /* Configure Output Pin B0 as TIM3C3 */
    gpio_set_mode(GPIOB, GPIO_MODE_OUTPUT_10_MHZ,
                  GPIO_CNF_OUTPUT_ALTFN_PUSHPULL, GPIO0);

    /* Enable Output compare mode */
    timer_enable_oc_output(TIM3, TIM_OC3);
}
