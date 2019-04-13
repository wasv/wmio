#include <stdint.h>

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define ADC_NCHAN 1
volatile uint16_t channel_values[ADC_NCHAN];

void adc_setup()
{
    /* Enable ADC1 clock. */
    rcc_periph_clock_enable(RCC_ADC1);

    /* Start ADC1 Powered Off */
    adc_power_off(ADC1);

    /* Set scan mode to support multiple analog inputs */
    adc_enable_scan_mode(ADC1);
    /* Set continuous mode */
    adc_set_continuous_conversion_mode(ADC1);
    /* Right align ADC reading */
    adc_set_left_aligned(ADC1);
    /* DMA request on ADC reading */
    adc_enable_dma(ADC1);

    uint8_t channel_array[ADC_NCHAN] = {0};
    /* Set channel sequence */
    adc_set_regular_sequence(ADC1, sizeof(channel_array), channel_array);
    //adc_set_sample_time_on_all_channels(ADC1, ADC_SMPR_SMP_239DOT5CYC);
    
    /* Enable GPIOA clock. */
    rcc_periph_clock_enable(RCC_GPIOA);

    /* Configure Output Pin A0 as ADC0 */
    gpio_set_mode(GPIOA, GPIO_MODE_INPUT,
                  GPIO_CNF_OUTPUT_ALTFN_OPENDRAIN, GPIO0);

    /* Power on ADC1 */
    adc_power_on(ADC1);

    /* 0.1s delay for ADC stabilization. */
    uint32_t ahb = rcc_ahb_frequency;
    for(uint32_t i = 0; i < ahb; i++)
        __asm__("nop");
    
    /* Calibrate ADC */
    adc_reset_calibration(ADC1);
    adc_calibrate(ADC1);

    adc_start_conversion_direct(ADC1);
}
