#ifndef _USB_IO_H_
#define _USB_IO_H_

#include <libopencm3/usb/usbd.h>
#include <stdint.h>

uint8_t usbd_control_buffer[128];

void usb_wmio_set_config(usbd_device *usbd_dev, uint16_t wValue);
void usb_wmio_data_rx_cb(usbd_device *usbd_dev, uint8_t ep);

const struct usb_device_descriptor dev;
const struct usb_config_descriptor config;

#endif
