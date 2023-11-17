#include "pico/unique_id.h"
#include "id.h"

static char id_buf[2 * PICO_UNIQUE_BOARD_ID_SIZE_BYTES + 1];

char *id_string(void) {
    pico_get_unique_board_id_string(id_buf, sizeof(id_buf));
    return id_buf;
}