#include "halt.h"

void halt_if(char *file, int line) {
    // TODO: return this to server
    printf("Halt at: %s, line %d\n", file, line);
    exit(-1);
}