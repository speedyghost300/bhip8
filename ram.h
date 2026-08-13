#ifndef RAM_H
#define RAM_H

#define TOTAL_RAM 4096
#define STACK_SIZE 16

#include <stdio.h>
#include <stdint.h>

uint8_t mem[TOTAL_RAM]; //4kB of memory, meaning 3 unique bytes for each address, so uint16_t when storing addresses
uint16_t* stack[STACK_SIZE];

//retrieve memory address (test function)
uint16_t* getMemAddr(uint16_t index);

//write to a memory address (test function)
void writeToMemory(uint16_t *addr, uint8_t data);

//load font into memory
void loadFont();

#endif
