/* Sets a MIDI control value over USB using a knob.  Sets an LEDs
 * brightness based on note velocity.  Uses PC13 for status LED, PB0
 * as PWM LED, PA0 as MIDI control (such as var. resistor)
 *
 * - William A Stevens V (wasv)
 */
#include "usb-io.h"
#include "periph.h"

#include <stdint.h>

#include <libopencm3/stm32/dma.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

usbd_device *usbd_dev;

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

    for(uint8_t i = 0; i < num_periph; i++) {
        if(periph_tbl[i]->id == buf[0]) {
            const wmio_periph_t* periph = periph_tbl[i];
            uint8_t chan = buf[1] >> 4;
            wmio_cmd_t cmd = buf[1] & 0xF;
            uint16_t data = buf[2] << 8 | buf[3];

            uint16_t state = 0;
            if(cmd > WMIO_CUSTOM) {
                periph->handle(chan, cmd, data);
                break;
            } else {
                switch(cmd) {
                case WMIO_READ:
                    state = periph->read(chan);

                    buf[3] = state & 0xFF;
                    buf[2] = state >> 8;
                    
                    usbd_ep_write_packet(usbd_dev, 0x81, buf, 4);
                    break;
                case WMIO_WRITE:
                    periph->write(chan, data);
                    break;
                case WMIO_CLEAR:
                    data = periph->read(chan) & data;
                    periph->write(chan, data);
                    break;
                case WMIO_SET:
                    data = periph->read(chan) | data;
                    periph->write(chan, data);
                    break;
                case WMIO_TOGGLE:
                    data = periph->read(chan) ^ ~data;
                    periph->write(chan, data);
                    break;
                default:
                    break;
                }
            } 
                         
        }
    }

}

int main(void)
{
    rcc_clock_setup_in_hse_8mhz_out_72mhz();

    usbd_dev = usbd_init(&st_usbfs_v1_usb_driver, &dev, &config,
                         usb_strings, 3,
                         usbd_control_buffer, sizeof(usbd_control_buffer));

    usbd_register_set_config_callback(usbd_dev, usb_wmio_set_config);

    for(uint8_t i = 0; i < num_periph; i++) {
        periph_tbl[i]->setup();
    }
    
    while (1) {
        usbd_poll(usbd_dev);
    }

    return 0;
}
