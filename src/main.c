/* Sets a MIDI control value over USB using a knob.  Sets an LEDs
 * brightness based on note velocity.  Uses PC13 for status LED, PB0
 * as PWM LED, PA0 as MIDI control (such as var. resistor)
 *
 * - William A Stevens V (wasv)
 */
#include "periph.h"
#include "usb-io.h"
#include "cbuf.h"

#include <stdint.h>

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#define NSAMP 4

usbd_device *usbd_dev;
volatile uint8_t adc0_update = false;

const char *usb_strings[] = {
    "wasv.me:wmio",
    "wasv.me",
    "WMIO",
};

void usb_wmio_data_rx_cb(usbd_device *usbd_dev, uint8_t ep)
{
    (void)ep;

    uint8_t buf[4];
    usbd_ep_read_packet(usbd_dev, 0x01, buf, 4);
    usbd_ep_write_packet(usbd_dev, 0x81, buf, 4);

    gpio_toggle(GPIOC, GPIO13);

}

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    gpio_setup();

    gpio_set(GPIOC, GPIO13);

    usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, &dev, &config,
                         usb_strings, 3,
                         usbd_control_buffer, sizeof(usbd_control_buffer));

    usbd_register_set_config_callback(usbd_dev, usb_wmio_set_config);

    gpio_clear(GPIOC, GPIO13);

    while (1) {
        usbd_poll(usbd_dev);
    }

    return 0;
}
