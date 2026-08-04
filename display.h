#ifndef DISPLAY_H
#define DISPLAY_H

#include "SDL3/SDL.h"
#include "res.h"

bool display[ORIGINAL_HEIGHT][ORIGINAL_WIDTH];

//initialize the display
void clearDisplay();

//draws pixel to screen with origin being top left corner
void drawPixel(SDL_Renderer* renderer, int y, int x);

//set a pixel at a specific coordinate (mapped to 2d array)
void setPixel(int row, int column);

//update the display on screen
void updateDisplay(SDL_Renderer* renderer);

#endif
