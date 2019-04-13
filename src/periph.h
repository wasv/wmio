#ifndef _PERIPH_H_
#define _PERIPH_H_

#include "periph/id.h"
#include "periph/led.h"

const wmio_periph_t* periph_tbl[] = {&id_periph, &led_periph};

const uint8_t num_periph = sizeof(periph_tbl)/sizeof(wmio_periph_t*);

#endif
