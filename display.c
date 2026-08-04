#include "display.h"

void clearDisplay() {
	//might not need since SDL has its own clear screen function, will determine later
	for (int x = 0; x < ORIGINAL_WIDTH; x++) {
		for (int y = 0; y < ORIGINAL_HEIGHT; y++) {
			display[y][x] = 0;
		}
	}
}

void drawPixel(SDL_Renderer* renderer, int y, int x) {
	//pixels are rendered left to right with vertical lines
	for (int i = x; i < x+PIXEL_WIDTH; i++) {
		SDL_RenderLine(renderer, i, y, i, y+PIXEL_HEIGHT);
	}
}

void setPixel(int row, int column) {
	//flip the pixel rather than setting to a certain value
	display[row][column] = !display[row][column];
}

void updateDisplay(SDL_Renderer* renderer) {
	SDL_RenderClear(renderer);

	//similar to individual pixels, display is rendered left to right
	for (int x = 0; x < ORIGINAL_WIDTH; x++) {
		for (int y = 0; y < ORIGINAL_HEIGHT; y++) {
			if (display[y][x] == 1) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
				drawPixel(renderer, y*SCALE, x*SCALE);
			}
			else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
				drawPixel(renderer, y * SCALE, x * SCALE);
			}
		}
	}

	SDL_RenderPresent(renderer);
}