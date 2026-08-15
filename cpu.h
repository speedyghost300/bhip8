#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "res.h"
#include "display.h"
#include "ram.h"
#include "timer.h"
#include "keypad.h"

struct CPU {
	uint8_t delay; //delay timer
	uint8_t sound; //sound timer
	int8_t sp; //stack pointer
	uint16_t *pc; //program counter
	uint16_t *i; //index register
	uint8_t v[16]; //general purpose variable registers
};
struct CPU* cpu;

enum OPCODES {
	CLEAR_SCREEN = 0x0, //also includes code 00E0 for returning from subroutine
	JUMP = 0x1,
	SUBROUTINE = 0x2,
	SKIP_E = 0x3,
	SKIP_NE = 0x4, 
	SKIP_XY_E = 0x5,
	SET_REG = 0x6,
	ADD_TO_REG = 0x7,
	LOGICAL = 0x8,
	SKIP_XY_NE = 0x9,
	SET_INDEX_REG = 0xA,
	JUMP_OFFSET = 0xB,
	RANDOM = 0xC,
	DISPLAY = 0xD,
	SKIP_IF_KEY = 0xE,
	MISC = 0xF
};

//set the program counter so that it points at a specific address
void setPC(struct CPU* cpu, uint16_t* addr);

//gets the instruction from memory pointed to by the program counter
uint16_t fetch(struct CPU* cpu);

//based on the fetched instruction, execute the command
void decode(struct CPU* cpu, uint16_t ins);

//initialize the stack
void initStack(struct CPU* cpu);

//push memory address to stack, if not full
void push(struct CPU* cpu, uint16_t* addr);

//pop most recent address from stack
uint16_t* pop(struct CPU* cpu);

//for decrementing delay and sound timers
void decreaseTimers(struct CPU* cpu);

#endif

