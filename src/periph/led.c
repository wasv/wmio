#include "led.h"

#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

void led_setup()
{
    /* Enable GPIOC clock. */
    rcc_periph_clock_enable(RCC_GPIOC);

    /* Set GPIO13 (in GPIOC) to 'output push-pull'. */
    gpio_set_mode(GPIOC, GPIO_MODE_OUTPUT_2_MHZ,
                  GPIO_CNF_OUTPUT_PUSHPULL, GPIO13);

    gpio_set(GPIOC, GPIO13);
}

void led_write(uint8_t chan, uint16_t data)
{
    (void) chan;

    if(data & 1) {
        gpio_clear(GPIOC, GPIO13);
    } else {
        gpio_set(GPIOC, GPIO13);
    }
}

uint16_t led_read(uint8_t chan)
{
    (void) chan;

    uint16_t state = gpio_get(GPIOC, GPIO13);
    return (state >> 13) & 1;
}

void led_handle(uint8_t chan, uint8_t cmd, uint16_t data)
{
    (void) chan;
    (void) cmd;
    (void) data;
    /* No custom commands, stubbed. */
}

const wmio_periph_t led_periph = {
    .id = 1,
    .setup  = led_setup,
    .write  = led_write,
    .read   = led_read,
    .handle = led_handle,
};
