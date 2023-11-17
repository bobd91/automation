#ifndef _HALT_H
#define _HALT_H

#define halt() halt_at(__FILE__, __LINE__)
#define halt_if(a) if(a) halt()

void halt_at(char *file, int line);

#endif