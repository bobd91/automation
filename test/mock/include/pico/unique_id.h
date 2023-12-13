#ifndef _MOCK_PICO_UNIQUE_ID_H__
#define _MOCK_PICO_UNIQUE_ID_H__

#include "mock/trace.h"
#include "pico/types.h"

#define PICO_UNIQUE_BOARD_ID_SIZE_BYTES 16

void pico_get_unique_board_id_string(char *id_out, uint len);

#endif