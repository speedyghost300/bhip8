#include "ram.h"

uint16_t* getMemAddr(uint16_t index) {
	printf("getMemAddr(): Points to address 0x%p with stored value 0x%X\n", &mem[index], mem[index]);
	return &mem[index];
}

void writeToMemory(uint16_t *addr, uint8_t data) {
	*addr = data;
	printf("writeToMemory(): Updated memory address 0x%p with data 0x%X\n", addr, *addr);
}

void loadFont() {
	writeToMemory(&mem[0x050], 0xF0); //0
	writeToMemory(&mem[0x051], 0x90);
	writeToMemory(&mem[0x052], 0x90);
	writeToMemory(&mem[0x053], 0x90);
	writeToMemory(&mem[0x054], 0xF0);

	writeToMemory(&mem[0x055], 0x20); //1
	writeToMemory(&mem[0x056], 0x60);
	writeToMemory(&mem[0x057], 0x20);
	writeToMemory(&mem[0x058], 0x20);
	writeToMemory(&mem[0x059], 0x70);

	writeToMemory(&mem[0x05A], 0xF0); //2
	writeToMemory(&mem[0x05B], 0x10);
	writeToMemory(&mem[0x05C], 0xF0);
	writeToMemory(&mem[0x05D], 0x80);
	writeToMemory(&mem[0x05E], 0xF0);

	writeToMemory(&mem[0x05F], 0xF0); //3
	writeToMemory(&mem[0x060], 0x10);
	writeToMemory(&mem[0x061], 0xF0);
	writeToMemory(&mem[0x062], 0x10);
	writeToMemory(&mem[0x063], 0xF0);

	writeToMemory(&mem[0x064], 0x90); //4
	writeToMemory(&mem[0x065], 0x90);
	writeToMemory(&mem[0x066], 0xF0);
	writeToMemory(&mem[0x067], 0x10);
	writeToMemory(&mem[0x068], 0x10);

	writeToMemory(&mem[0x069], 0xF0); //5
	writeToMemory(&mem[0x06A], 0x80);
	writeToMemory(&mem[0x06B], 0xF0);
	writeToMemory(&mem[0x06C], 0x10);
	writeToMemory(&mem[0x06D], 0xF0);

	writeToMemory(&mem[0x06E], 0xF0); //6
	writeToMemory(&mem[0x06F], 0x80);
	writeToMemory(&mem[0x070], 0xF0);
	writeToMemory(&mem[0x071], 0x90);
	writeToMemory(&mem[0x072], 0xF0);

	writeToMemory(&mem[0x073], 0xF0); //7
	writeToMemory(&mem[0x074], 0x10);
	writeToMemory(&mem[0x075], 0x20);
	writeToMemory(&mem[0x076], 0x40);
	writeToMemory(&mem[0x077], 0x40);

	writeToMemory(&mem[0x078], 0xF0); //8
	writeToMemory(&mem[0x079], 0x90);
	writeToMemory(&mem[0x07A], 0xF0);
	writeToMemory(&mem[0x07B], 0x90);
	writeToMemory(&mem[0x07C], 0xF0);

	writeToMemory(&mem[0x07D], 0xF0); //9
	writeToMemory(&mem[0x07E], 0x90);
	writeToMemory(&mem[0x07F], 0xF0);
	writeToMemory(&mem[0x080], 0x10);
	writeToMemory(&mem[0x081], 0xF0);

	writeToMemory(&mem[0x082], 0xF0); //A
	writeToMemory(&mem[0x083], 0x90);
	writeToMemory(&mem[0x084], 0xF0);
	writeToMemory(&mem[0x085], 0x90);
	writeToMemory(&mem[0x086], 0x90);

	writeToMemory(&mem[0x087], 0xE0); //B
	writeToMemory(&mem[0x088], 0x90);
	writeToMemory(&mem[0x089], 0xE0);
	writeToMemory(&mem[0x08A], 0x90);
	writeToMemory(&mem[0x08B], 0xE0);

	writeToMemory(&mem[0x08C], 0xF0); //C
	writeToMemory(&mem[0x08D], 0x80);
	writeToMemory(&mem[0x08E], 0x80);
	writeToMemory(&mem[0x08F], 0x80);
	writeToMemory(&mem[0x090], 0xF0);

	writeToMemory(&mem[0x091], 0xE0); //D
	writeToMemory(&mem[0x092], 0x90);
	writeToMemory(&mem[0x093], 0x90);
	writeToMemory(&mem[0x094], 0x90);
	writeToMemory(&mem[0x095], 0xE0);

	writeToMemory(&mem[0x096], 0xF0); //E
	writeToMemory(&mem[0x097], 0x80);
	writeToMemory(&mem[0x098], 0xF0);
	writeToMemory(&mem[0x099], 0x80);
	writeToMemory(&mem[0x09A], 0xF0);

	writeToMemory(&mem[0x09B], 0xF0); //F
	writeToMemory(&mem[0x09C], 0x80);
	writeToMemory(&mem[0x09D], 0xF0);
	writeToMemory(&mem[0x09E], 0x80);
	writeToMemory(&mem[0x09F], 0x80);

	printf("loadFont(): font loaded into memory\n");
}