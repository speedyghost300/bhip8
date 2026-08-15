#include "keypad.h"

uint8_t keyPressed() {
	const bool* keyState = SDL_GetKeyboardState(NULL);
	uint8_t hex = 0xFF;

	if (keyState[SDL_SCANCODE_1]) {
		hex = 0x01;
	}
	if (keyState[SDL_SCANCODE_2]) {
		hex = 0x02;
	}
	if (keyState[SDL_SCANCODE_3]) {
		hex = 0x03;
	}
	if (keyState[SDL_SCANCODE_4]) {
		hex = 0x0C;
	}
	if (keyState[SDL_SCANCODE_Q]) {
		hex = 0x04;
	}
	if (keyState[SDL_SCANCODE_W]) {
		hex = 0x05;
	}
	if (keyState[SDL_SCANCODE_E]) {
		hex = 0x06;
	}
	if (keyState[SDL_SCANCODE_R]) {
		hex = 0x0D;
	}
	if (keyState[SDL_SCANCODE_A]) {
		hex = 0x07;
	}
	if (keyState[SDL_SCANCODE_S]) {
		hex = 0x08;
	}
	if (keyState[SDL_SCANCODE_D]) {
		hex = 0x09;
	}
	if (keyState[SDL_SCANCODE_F]) {
		hex = 0x0E;
	}
	if (keyState[SDL_SCANCODE_Z]) {
		hex = 0x0A;
	}
	if (keyState[SDL_SCANCODE_X]) {
		hex = 0x00;
	}
	if (keyState[SDL_SCANCODE_C]) {
		hex = 0x0B;
	}
	if (keyState[SDL_SCANCODE_V]) {
		hex = 0x0F;
	}

	return hex;
}
