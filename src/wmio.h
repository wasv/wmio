#ifndef _WMIO_H_
#define _WMIO_H_
#include <stdint.h>

typedef enum { WMIO_READ = 0, WMIO_WRITE = 1, WMIO_CLEAR = 2,
               WMIO_SET = 3, WMIO_TOGGLE = 4, WMIO_CUSTOM = 7} wmio_cmd_t;

// Type for custom command handler function pointer.
typedef void (*wmio_handle_f)(uint8_t /*chan*/, uint8_t /*cmd*/, uint16_t /*data*/);

// Type for read handler function pointer.
typedef uint16_t (*wmio_read_f)(uint8_t /*chan*/);

// Type for write handler function pointer.
typedef void (*wmio_write_f)(uint8_t /*chan*/, uint16_t /*data*/);

// Type for read handler function pointer.
typedef void (*wmio_setup_f)();

// Struct type for peripheral objects.
typedef struct {
    uint8_t id;
    wmio_setup_f setup;
    wmio_handle_f handle;
    wmio_read_f read;
    wmio_write_f write;
} wmio_periph_t;
#endif
