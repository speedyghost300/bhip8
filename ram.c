#include "ram.h"

uint16_t* getMemAddr(uint16_t index) {
	printf("getMemAddr(): Points to address 0x%p with stored value 0x%X\n", &mem[index], mem[index]);
	return &mem[index];
}

void writeToMemory(uint16_t *addr, uint8_t data) {
	*addr = data;
	printf("writeToMemory(): Updated memory address 0x%p with data 0x%X\n", addr, *addr);
}

void initStack(struct Stack* stack) {
	stack->sp = -1;
	printf("initStack(): Stack pointer points to index %X\n", stack->sp);
}

void push(struct Stack* stack, uint16_t* addr) {
	if (stack->sp >= STACK_SIZE - 1) {
		printf("push(): The stack is full!\n");
	}
	else {
		stack->sp += 1;
		stack->arr[stack->sp] = addr;
		printf("push(): Pushed memory address 0x%p to stack index %X\n", stack->arr[stack->sp], stack->sp);
	}
}

uint16_t* pop(struct Stack* stack) {
	if (stack->sp < 0) {
		printf("pop(): The stack is empty!\n");
		return NULL;
	}
	else {
		printf("pop(): Popped memory address 0x%p from stack index %X\n", stack->arr[stack->sp], stack->sp);
		stack->sp -= 1;
		return stack->arr[stack->sp + 1];
	}
}

uint16_t* peek(struct Stack* stack) {
	if (stack->sp < 0) {
		printf("peek(): The stack is empty!\n");
		return NULL;
	}
	else {
		printf("peek(): Stack currently points to address 0x%p at index %X\n", stack->arr[stack->sp], stack->sp);
		return stack->arr[stack->sp];
	}
}