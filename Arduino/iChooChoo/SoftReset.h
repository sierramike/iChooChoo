#ifndef _SOFT_RESTART_H
#define _SOFT_RESTART_H

#include <avr/wdt.h>

#define soft_restart()      \
do                          \
{                           \
    wdt_enable(WDTO_4S);    \
    for(;;)                 \
    {                       \
    }                       \
} while(0)

#endif
