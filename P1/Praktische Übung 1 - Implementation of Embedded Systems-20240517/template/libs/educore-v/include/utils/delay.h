#ifndef __UTILS__DELAY_H__
#define __UTILS__DELAY_H__

#include <stdint.h>
#include <risc-v.h>

#define _delay_ms(MS) { \
    uint32_t __ms = MS * 4200;\
    while(__ms--) nop; \
}

#endif // __UTILS__DELAY_H__
