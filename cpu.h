#ifndef CPU_H
#define CPU_H

#include <stdio.h>
#include <stdint.h>
#include "res.h"
#include "display.h"
#include "ram.h"
#include "timer.h"

struct CPU {
	uint16_t *pc; //program counter
	uint16_t *i; //index register
	uint8_t v[16]; //general purpose variable registers
};
struct CPU* cpu;

enum OPCODES {
	CLEAR_SCREEN = 0x0,
	JUMP = 0x1,
	SET_REG = 0x6,
	ADD_TO_REG = 0x7,
	SET_INDEX_REG = 0xA,
	DISPLAY = 0xD
};

//set the program counter so that it points at a specific address
void setPC(struct CPU* cpu, uint16_t* addr);

//gets the instruction from memory pointed to by the program counter
uint16_t fetch(struct CPU* cpu);

//based on the fetched instruction, execute the command
void decode(struct CPU* cpu, uint16_t ins);

#endif

