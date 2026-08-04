#include "tests.h"

void memTest() {
	//test stack init
	printf("INITIALIZING THE STACK:\n");
	initStack(&stack);
	printf("\n");

	//test r/w to RAM
	printf("ADDRESS R/W TESTS:\n");
	uint16_t* addr;
	addr = getMemAddr(mem[0x200]);
	writeToMemory(&mem[0x200], 0x1F);
	addr = getMemAddr(mem[0x201]);
	writeToMemory(&mem[0x201], 0xA0);
	addr = getMemAddr(mem[0x200]);
	printf("\n");

	//test push/pop/peek for stack
	printf("STACK TESTING:\n");

	//try to pop/peek from an empty stack
	pop(&stack);
	peek(&stack);
	printf("\n");

	//push memory address to stack, change address, then push again
	push(&stack, addr);
	addr = getMemAddr(0x1FF);
	push(&stack, addr);
	printf("\n");

	//check to see if correct memory address has been pushed to correct index
	peek(&stack);
	printf("\n");

	//pop from stack until empty and peek each time
	pop(&stack);
	peek(&stack);
	pop(&stack);
	peek(&stack);
	printf("\n");

	//push to stack until full then
	push(&stack, addr);
	for (int i = 0; i < STACK_SIZE; i++) {
		push(&stack, addr);
	}
	peek(&stack);
	printf("\n");
}

void cpuTest() {
	//program counter testing
	printf("PROGRAM COUNTER TESTS:\n");
	uint16_t* addr = getMemAddr(mem[0x200]);
	setPC(&cpu, &mem[0x200]);
	uint16_t ins = fetch(&cpu);
	decode(&cpu, ins);
	ins = fetch(&cpu);
	decode(&cpu, ins);
}

void drawTest() {
	//test drawing to screen
	setPixel(0, 0);
	setPixel(10, 10);
}