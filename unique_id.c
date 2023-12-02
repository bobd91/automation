#include "pico/unique_id.h"
#include "unique_id.h"

static char id_buf[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

char *unique_id(void) {
    pico_get_unique_board_id_string(id_buf, sizeof(id_buf));
    return id_buf;
}