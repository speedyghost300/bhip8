#ifndef RAM_H
#define RAM_H

#define TOTAL_RAM 4096
#define STACK_SIZE 16

#include <stdio.h>
#include <stdint.h>

uint8_t mem[TOTAL_RAM]; //4kB of memory, meaning 3 unique bytes for each address, so uint16_t when storing addresses

struct Stack {
	uint16_t* arr[STACK_SIZE];
	int sp; //stack pointer
};
struct Stack* stack;

//retrieve memory address
uint16_t* getMemAddr(uint16_t index);

//write to a memory address
void writeToMemory(uint16_t *addr, uint8_t data);

//initialize the stack
void initStack(struct Stack* stack);

//push memory address to stack, if not full
void push(struct Stack* stack, uint16_t* addr);

//pop most recent address from stack
uint16_t* pop(struct Stack* stack);

//view most recent address on stack without popping it
uint16_t* peek(struct Stack* stack);

#endif
