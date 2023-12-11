#include "pico/unique_id.h"

static const char *board_id = "MOCKBOARD1";

void pico_get_unique_board_id_string(char *id_out, uint len) {
    assert(id_out);
    assert(len > strlen(board_id));
    MOCK_TRACE("id_out, %u", len);

    strcpy(id_out, board_id);
}