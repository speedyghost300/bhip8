#ifndef TIMER_H
#define TIMER_H

#include <stdint.h>

#define DISPLAY_HZ 60
#define CPU_HZ 10

#define ONE_SECOND 1000000000
#define CPU_CLK (ONE_SECOND / CPU_HZ)
#define TIMER_CLK (ONE_SECOND / DISPLAY_HZ)

uint8_t delay;
uint8_t sound;

#endif