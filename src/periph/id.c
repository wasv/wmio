#include "id.h"

void id_setup()
{
}

void id_write(uint8_t chan, uint16_t data)
{
    (void) chan;
    (void) data;
    /* Read only. Stubbed */
}

uint16_t id_read(uint8_t chan)
{
    (void) chan;
    return 0x4242;
}

void id_handle(uint8_t chan, uint8_t cmd, uint16_t data)
{
    (void) chan;
    (void) cmd;
    (void) data;
    /* No custom commands, stubbed. */
}

const wmio_periph_t id_periph = {
    .id = 0,
    .setup  = id_setup,
    .write  = id_write,
    .read   = id_read,
    .handle = id_handle,
};
