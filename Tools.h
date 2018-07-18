/*
    kbhit() implementation retrieved from: https://www.linuxjournal.com/article/1138
    Written by Alan Cox
    Published 1 Sept 1995
*/

#include <sys/select.h>

int kbhit(void);
