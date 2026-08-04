#include "cpu.h"

void setPC(struct CPU* cpu, uint16_t* addr) {
	cpu->pc = addr;
	printf("setPC(): Program counter now points to memory address 0x%p\n", cpu->pc);
}

uint16_t fetch(struct CPU* cpu) {
	uint16_t ins; //full 16-bit instruction
	ins = *cpu->pc;

	//chip 8 instructions are big endian, so we need to fix
	uint8_t ins0, ins1;
	ins0 = ins >> 8; //little byte
	ins1 = ins; //big byte
	ins = (ins1 << 8) | ins0; //combine bytes again

	printf("fetch(): instruction is 0x%2X\n", ins);

	//increment pc by 2 bytes
	setPC(cpu, cpu->pc+1);
	return ins;
}

void decode(struct CPU* cpu, uint16_t ins) {
	uint8_t n0, n1, n2, n3; //half bytes
	n0 = (ins & 0xF000) >> 12;
	n1 = (ins & 0x0F00) >> 8;
	n2 = (ins & 0x00F0) >> 4;
	n3 = (ins & 0x000F);
	printf("decode(): first nibble is 0x%X ", n0);

	switch (n0) {
	case CLEAR_SCREEN:
		printf("command: clear screen\n");
		clearDisplay();
		break;
	case JUMP:
		uint16_t addr = (n1 << 8) | (n2 << 4) | (n3);
		printf("command: jump to addr 0x%p\n", &mem[addr]);
		setPC(cpu, &mem[addr]);
		break;
	case SET_REG:
		uint8_t val = (n2 << 4) | (n3);
		cpu->v[n1] = val; //set register based on nibble1 to val
		printf("command: set register v[%X] with value 0x%X\n", n1, cpu->v[n1]);
		break;
	case ADD_TO_REG:
		val = (n2 << 4) | (n3);
		cpu->v[n1] += val; //add val to register based on nibble1
		printf("command: added val %X to register v[%X] with new value 0x%X\n", val, n1, cpu->v[n1]);
		break;
	case SET_INDEX_REG:
		addr = (n1 << 8) | (n2 << 4) | (n3);
		cpu->i = &mem[addr];
		printf("command: set index register to address 0x%p\n", cpu->i);
		break;
	case DISPLAY:
		printf("command: draw to screen\n");
		//get x and y values from corresponding registers, modulo by display w/h to deal with wrapping sprites
		uint8_t x = cpu->v[n1] % 64;
		uint8_t y = cpu->v[n2] % 32;

		//always set v[F] to 0
		cpu->v[0xF] = 0;
		
		//nibble3 is the number of rows
		for (int j = 0; j < n3; j++) {
			//get the jth byte from the index reg
			uint8_t byte = *(cpu->i + j);
			
			for (int k = 0; k < 8; k++) {
				uint8_t bit = (byte << k) & 0b10000000; //bitmask each individual bit in byte (MSB to LSB)

				//if x goes past the screen res, break the inner loop
				if (x >= ORIGINAL_WIDTH) {
					break;
				}

				//if pixel is on and bit is 1, set v[F] to 1
				if (display[y][x] == 1 && bit == 128) {
					setPixel(y, x);
					cpu->v[0xF] = 1;
				}
				else if (display[y][x] == 0 && bit == 128) {
					setPixel(y, x);
				}

				x++;
			}
			
			y++;
			x = cpu->v[n1] % 64;

			//stop if we reach the bottom edge
			if (y >= ORIGINAL_HEIGHT || x >= ORIGINAL_WIDTH) {
				break;
			}
		}

		break;
	default:
		printf("\n");
		break;
	}
	printf("\n");
}