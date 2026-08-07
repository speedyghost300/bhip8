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

	printf("fetch(): instruction is 0x%2X (0x%p)\n", ins, cpu->pc);

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

	uint16_t addr = (n1 << 8) | (n2 << 4) | (n3); //variable used to store memory addresses
	uint8_t val = (n2 << 4) | (n3); //variable used to store single bytes

	switch (n0) {
	case CLEAR_SCREEN:
		switch (n3) {
		case 0x0: //clear screen (00E0)
			printf("command: clear screen\n");
			clearDisplay();
			break;
		case 0xE: //return from subroutine (00EE)
			printf("command: returning from subroutine\n");
			setPC(cpu, pop(&stack)); //pop memory address from stack
			break;
		}
		break;
	case JUMP:
		printf("command: jump to addr 0x%p\n", &mem[addr]);
		setPC(cpu, &mem[addr]);
		break;
	case SUBROUTINE:
		printf("command: subroutine called at 0x%p\n", cpu->pc);
		push(&stack, cpu->pc); //push current address in pc to stack, then set pc to addr
		setPC(cpu, &mem[addr]);
		break;
	case SKIP_E:
		//code 3XNN: if value stored in register v[x] is equal to byte 0xNN, skip instruction
		if (cpu->v[n1] == val) {
			printf("command: skip instruction at addr 0x%p, move to addr 0x%p\n", cpu->pc, cpu->pc+1);
			setPC(cpu, cpu->pc+1);
		}
		else {
			printf("command: condiditon not true, do not skip instruction\n");
		}
		break;
	case SKIP_NE:
		//code 4XNN: if value stored in register v[x] is NOT equal to byte 0xNN, skip instruction
		if (cpu->v[n1] != val) {
			printf("command: skip instruction at addr 0x%p, move to addr 0x%p\n", cpu->pc, cpu->pc+1);
			setPC(cpu, cpu->pc+1);
		}
		else {
			printf("command: condiditon not true, do not skip instruction\n");
		}
		break;
	case SKIP_XY_E:
		//code 5XY0: if value in v[x] is equal to value in v[y], skip instruction
		if (cpu->v[n1] == cpu->v[n2]) {
			printf("command: skip instruction at addr 0x%p, move to addr 0x%p\n", cpu->pc, cpu->pc+1);
			setPC(cpu, cpu->pc+1);
		}
		else {
			printf("command: condiditon not true, do not skip instruction\n");
		}
		break;
	case SKIP_XY_NE:
		//code 9XY0: if value in v[x] is NOT equal to value in v[y], skip instruction
		if (cpu->v[n1] != cpu->v[n2]) {
			printf("command: skip instruction at addr 0x%p, move to addr 0x%p\n", cpu->pc, cpu->pc+1);
			setPC(cpu, cpu->pc+1);
		}
		else {
			printf("command: condiditon not true, do not skip instruction\n");
		}
		break;
	case SET_REG:
		cpu->v[n1] = val; //set register based on nibble1 to val
		printf("command: set register v[%X] with value 0x%X\n", n1, cpu->v[n1]);
		break;
	case ADD_TO_REG:
		cpu->v[n1] += val; //add val to register based on nibble1
		printf("command: added val %X to register v[%X] with new value 0x%X\n", val, n1, cpu->v[n1]);
		break;
	case LOGICAL:
		switch (n3) { //logical instruction depends on nibble3
		case 0x0: //code 8XY0: set v[X] to value in v[Y]
			cpu->v[n1] = cpu->v[n2];
			printf("command: register v[%X] to value stored in v[%X], value stored is 0x%X\n", n1, n2, cpu->v[n1]);
			break;
		case 0x1: //code 8XY1: v[X] = v[X] OR v[Y]
			cpu->v[n1] = (cpu->v[n1] | cpu->v[n2]);
			printf("command: register v[%X] logical OR with v[%X], new value is 0x%X\n", n1, n2, cpu->v[n1]);
			break;
		case 0x2: //code 8XY2: v[X] = v[X] AND v[Y]
			cpu->v[n1] = (cpu->v[n1] & cpu->v[n2]);
			printf("command: register v[%X] logical AND with v[%X], new value is 0x%X\n", n1, n2, cpu->v[n1]);
			break;
		case 0x3: //code 8XY3: v[X] = v[X] XOR v[Y]
			cpu->v[n1] = (cpu->v[n1] ^ cpu->v[n2]);
			printf("command: register v[%X] logical XOR with v[%X], new value is 0x%X\n", n1, n2, cpu->v[n1]);
			break;
		case 0x4: //code 8XY4: v[X] = v[X] + v[Y]
			if ((cpu->v[n1] + cpu->v[n2]) > 255) { //if value > 255 (0xFF), set v[F] to 1
				cpu->v[0xF] = 1;
			}
			cpu->v[n1] += cpu->v[n2];
			printf("command: add value of register v[%X] to register v[%X], new value is 0x%X\n", n2, n1, cpu->v[n1]);
			break;
		case 0x5: //code 8XY5: v[X] = v[X] - v[Y]
			if (cpu->v[n1] >= cpu->v[n2]) {
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			cpu->v[n1] -= cpu->v[n2];
			printf("command: subtract value of register v[%X] from register v[%X], new value is 0x%X\n", n2, n1, cpu->v[n1]);
			break;
		case 0x7: //code 8XY7: v[X] = v[Y] - v[X]
			if (cpu->v[n2] >= cpu->v[n1]) {
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			cpu->v[n1] = cpu->v[n2] - cpu->v[n1];
			printf("command: subtract value of register v[%X] from register v[%X], new value is 0x%X\n", n1, n2, cpu->v[n1]);
			break;
		case 0x6: //bit shift once to the right
			if (cpu->v[n1] & 0b00000001 == 1) { //if shifted bit is 1, set v[F] to 1, else set to 0
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			cpu->v[n1] = cpu->v[n1] >> 1;
			break;
		case 0xE: //bit shift once to the left
			if (cpu->v[n1] & 0b10000000 == 128) { //if shifted bit is 1, set v[F] to 1, else set to 0
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			cpu->v[n1] = cpu->v[n1] << 1;
			break;
		}
		break;
	case SET_INDEX_REG:
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
			uint8_t byte = *((uint8_t*)cpu->i + j);

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

			//stop if we reach the bottom edge
			if (y >= ORIGINAL_HEIGHT) {
				break;
			}
			x = cpu->v[n1] % 64;
		}

		break;
	default:
		printf("\n");
		break;
	}
	printf("\n");
}