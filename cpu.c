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
	uint8_t key; //for key presses, set default as 0xFF

	switch (n0) {
	case CLEAR_SCREEN:
		switch (n3) {
		case 0x0: //clear screen (00E0)
			printf("command: clear screen\n");
			clearDisplay();
			break;
		case 0xE: //return from subroutine (00EE)
			printf("command: returning from subroutine\n");
			setPC(cpu, pop(cpu)); //pop memory address from stack
			break;
		}
		break;
	case JUMP:
		printf("command: jump to addr 0x%p\n", &mem[addr]);
		setPC(cpu, &mem[addr]);
		break;
	case SUBROUTINE:
		printf("command: subroutine called at 0x%p\n", cpu->pc);
		push(cpu, cpu->pc); //push current address in pc to stack, then set pc to addr
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
			uint16_t sum = cpu->v[n1] + cpu->v[n2];
			cpu->v[n1] = sum & 0xFF; //preform the operation first, then set flags
			if (sum > 255) { //if value > 255 (0xFF), set v[F] to 1
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			printf("command: add value of register v[%X] to register v[%X], new value is 0x%X\n", n2, n1, cpu->v[n1]);
			break;
		case 0x5: //code 8XY5: v[X] = v[X] - v[Y]
			uint8_t x = cpu->v[n1];
			cpu->v[n1] -= cpu->v[n2];
			if (x >= cpu->v[n2]) {
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			printf("command: subtract value of register v[%X] from register v[%X], new value is 0x%X\n", n2, n1, cpu->v[n1]);
			break;
		case 0x7: //code 8XY7: v[X] = v[Y] - v[X]
			uint8_t y = cpu->v[n2];
			cpu->v[n1] = cpu->v[n2] - cpu->v[n1];
			if (y >= cpu->v[n1]) {
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			printf("command: subtract value of register v[%X] from register v[%X], new value is 0x%X\n", n1, n2, cpu->v[n1]);
			break;
		case 0x6: //code 8XY6: bit shift once to the right
			x = cpu->v[n1];
			cpu->v[n1] = cpu->v[n1] >> 1;
			if (x & 0b00000001 == 1) { //if shifted bit is 1, set v[F] to 1, else set to 0
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			printf("command: bit shift register v[%X] to the right, new value is 0x%X\n", n1, cpu->v[n1]);
			break;
		case 0xE: //code 8XYE: bit shift once to the left
			x = cpu->v[n1];
			cpu->v[n1] = cpu->v[n1] << 1;
			if (x & 0b10000000 == 128) { //if shifted bit is 1, set v[F] to 1, else set to 0
				cpu->v[0xF] = 1;
			}
			else {
				cpu->v[0xF] = 0;
			}
			printf("command: bit shift register v[%X] to the left, new value is 0x%X\n", n1, cpu->v[n1]);
			break;
		}
		break;
	case SET_INDEX_REG:
		cpu->i = &mem[addr];
		printf("command: set index register to address 0x%p\n", cpu->i);
		break;
	case JUMP_OFFSET:
		printf("command: jump with offset 0x%X to addr 0x%p\n", cpu->v[0x0], &mem[(addr+cpu->v[0x0])]);
		setPC(cpu, &mem[(addr+cpu->v[0x0])]);
		break;
	case RANDOM:
		cpu->v[n1] = (((rand() % val) + 1) & val);
		printf("store random number 0x%X in v[%X]\n", cpu->v[n1], n1);
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
	case SKIP_IF_KEY:
		key = keyPressed(); //get key scancode
		switch (n3) {
		case 0xE: //code EX9E: skip next ins if scancode is equal to value in v[X]
			if (key == cpu->v[n1]) {
				printf("command: key matches v[%X], skip instruction at addr 0x%p, move to addr 0x%p\n", n1, cpu->pc, cpu->pc+1);
				setPC(cpu, cpu->pc+1);
			}
			else {
				printf("command: condiditon not true, do not skip instruction\n");
			}
			break;
		case 0x1: //code EXA1: skip next ins if scancode value in v[X] is NOT pressed
			if (key != cpu->v[n1]) {
				printf("command: key does not match v[%X], skip instruction at addr 0x%p, move to addr 0x%p\n", n1, cpu->pc, cpu->pc+1);
				setPC(cpu, cpu->pc+1);
			}
			else {
				printf("command: condiditon not true, do not skip instruction\n");
			}
			break;
		}
		break;
	case MISC:
		key = keyPressed();
		switch (n3) {
		case 0x7: //code FX07: set v[X] to value in delay timer
			cpu->v[n1] = cpu->delay;
			printf("command: set register v[%X] to value stored in delay, new value %X\n", n1, cpu->v[n1]);
			break;
		case 0x5:
			switch (n2) {
			case 0x1: //code FX15: set delay timer to value stored in v[X]
				cpu->delay = cpu->v[n1];
				printf("command: set delay timer to value stored in register v[%X], new value %X\n", n1, cpu->delay);
				break;
			case 0x5: //code FX55: store values from v[0]-v[X] in memory, starting from index
				for (int reg = 0; reg <= n1; reg++) {
					*((uint8_t*)cpu->i+reg) = cpu->v[reg];
					printf("command: store value 0x%X from register v[%X] at addr 0x%p\n", cpu->v[reg], reg, (uint8_t*)(cpu->i+reg));
				}
				break;
			case 0x6: //code FX65: load values for index to v[0]-v[X]
				for (int reg = 0; reg <= n1; reg++) {
					cpu->v[reg] = *((uint8_t*)cpu->i+reg);
					printf("command: load value 0x%X from addr 0x%p to register v[%X]\n", *(uint8_t*)(cpu->i+reg), (uint8_t*)(cpu->i+reg), reg);
				}
				break;
			}
			break;
		case 0x8:
			cpu->sound = cpu->v[n1];
			printf("command: set sound timer to value stored in register v[%X], new value %X\n", n1, cpu->sound);
			break;
		case 0xE: //code FX1E: add v[X] to index
			(uint8_t*)cpu->i += cpu->v[n1];
			printf("command: add value stored in v[%X] to index, new addr 0x%p\n", n1, cpu->i);
			break;
		case 0xA: //code FX0A: set v[X] to scancode if key is pressed, otherwise repeat instruction
			if (key != 0xFF) {
				cpu->v[n1] = key;
				printf("command: set register v[%X] to scancode, scancode is 0x%X\n", n1, cpu->v[n1]);
			}
			else {
				setPC(cpu, cpu->pc-1); //repeat instruction
				printf("command: repeating instruction\n");
			}
			break;
		case 0x3: //code FX33: take number in v[X] and convert to decimal
			*(uint8_t*)cpu->i = cpu->v[n1] / 100;
			*((uint8_t*)cpu->i+1) = (cpu->v[n1] / 10) % 10;
			*((uint8_t*)cpu->i+2) = cpu->v[n1] % 10;
			printf("command: convert value stored in v[%X] to decimal, value is 0x%X\n", n1, cpu->v[n1]);
			break;
		case 0x9: //code FX29: load font character
			switch (n1) {
			case 0x0:
				cpu->i = &mem[0x050];
				break;
			case 0x1:
				cpu->i = &mem[0x055];
				break;
			case 0x2:
				cpu->i = &mem[0x05A];
				break;
			case 0x3:
				cpu->i = &mem[0x05F];
				break;
			case 0x4:
				cpu->i = &mem[0x064];
				break;
			case 0x5:
				cpu->i = &mem[0x069];
				break;
			case 0x6:
				cpu->i = &mem[0x06E];
				break;
			case 0x7:
				cpu->i = &mem[0x073];
				break;
			case 0x8:
				cpu->i = &mem[0x078];
				break;
			case 0x9:
				cpu->i = &mem[0x07D];
				break;
			case 0xA:
				cpu->i = &mem[0x082];
				break;
			case 0xB:
				cpu->i = &mem[0x087];
				break;
			case 0xC:
				cpu->i = &mem[0x08C];
				break;
			case 0xD:
				cpu->i = &mem[0x091];
				break;
			case 0xE:
				cpu->i = &mem[0x096];
				break;
			case 0xF:
				cpu->i = &mem[0x09B];
				break;
			}
			printf("command: loaded font character\n");
			break;
		}
		break;
	default:
		printf("\n");
		break;
	}
	printf("\n");
}

void initStack(struct CPU* cpu) {
	cpu->sp = -1;
	printf("initStack(): Stack pointer points to index %X\n", cpu->sp);
}

void push(struct CPU* cpu, uint16_t* addr) {
	if (cpu->sp >= STACK_SIZE - 1) {
		printf("push(): The stack is full!\n");
	}
	else {
		cpu->sp++;
		stack[cpu->sp] = addr;
		printf("push(): Pushed memory address 0x%p to stack index %X\n", stack[cpu->sp], cpu->sp);
	}
}

uint16_t* pop(struct CPU* cpu) {
	if (cpu->sp < 0) {
		printf("pop(): The stack is empty!\n");
		return NULL;
	}
	else {
		printf("pop(): Popped memory address 0x%p from stack index %X\n", stack[cpu->sp], cpu->sp);
		uint16_t* addr = stack[cpu->sp]; //can't do pointer math in the return statement so have to use a temp variable
		cpu->sp--;
		return addr;
	}
}

void decreaseTimers(struct CPU* cpu) {
	if (cpu->delay > 0) {
		cpu->delay--;
	}
	if (cpu->sound > 0) {
		cpu->sound--;
	}
}