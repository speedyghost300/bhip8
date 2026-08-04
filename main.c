#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include "SDL3/SDL.h"
#include "res.h"
#include "cpu.h"
#include "ram.h"
#include "display.h"
#include "timer.h"
#include "tests.h"

bool loadFile(const char* fileName, uint8_t* memory) {
	FILE* file;
	file = fopen(fileName, "rb");

	if (file == NULL) {
		printf("Unable to open the file\n");
		return false;
	}
	
	//read starting from 0x200
	fread(&memory[0x200], sizeof(uint8_t), 3584, file);

	fclose(file);
	return true;
}

void init() {
	memTest();
	drawTest();
	cpuTest();
}

int main() {
	//create window and renderer pointers
	SDL_Window* window;
	SDL_Renderer* renderer;
	bool quit = false;

	//init SDL and create window and renderer
	SDL_Init(SDL_INIT_VIDEO);
	window = SDL_CreateWindow("BHIP8", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL);
	
	if (window == NULL) {
		return -1;
	}

	renderer = SDL_CreateRenderer(window, NULL);

	if (renderer == NULL) {
		return -1;
	}

	uint64_t currentTime, lastTime, deltaTime;
	lastTime = SDL_GetTicksNS();

	clearDisplay(renderer);
	setPC(&cpu, &mem[0x200]);

	char fileName[30];
	printf("Enter the file name that you would like to load: ");
	if (scanf("%s", fileName) == 1) {
		loadFile(fileName, mem);
	}
	
	//main program loop
	while (!quit) {
		currentTime = SDL_GetTicksNS();
		deltaTime = currentTime - lastTime;

		SDL_Event event;

		if (deltaTime >= CPU_CLK) {
			lastTime = currentTime;
			uint16_t ins = fetch(&cpu);
			decode(&cpu, ins);
		}
		updateDisplay(renderer);

		while (SDL_PollEvent(&event)) {
			if (event.type == SDL_EVENT_QUIT) {
				quit = true;
			}
		}
	}

	//destroy window and renderer, then close SDL
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}