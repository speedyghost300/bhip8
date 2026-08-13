#include "keypad.h"

int keyPressed() {
	const bool* keyState = SDL_GetKeyboardState(NULL);
	int scancode = 0;

	if (keyState[SDL_SCANCODE_1]) {
		scancode = SDL_SCANCODE_1;
	}
	if (keyState[SDL_SCANCODE_2]) {
		scancode = SDL_SCANCODE_2;
	}
	if (keyState[SDL_SCANCODE_3]) {
		scancode = SDL_SCANCODE_3;
	}
	if (keyState[SDL_SCANCODE_4]) {
		scancode = SDL_SCANCODE_4;
	}
	if (keyState[SDL_SCANCODE_Q]) {
		scancode = SDL_SCANCODE_Q;
	}
	if (keyState[SDL_SCANCODE_W]) {
		scancode = SDL_SCANCODE_W;
	}
	if (keyState[SDL_SCANCODE_E]) {
		scancode = SDL_SCANCODE_E;
	}
	if (keyState[SDL_SCANCODE_R]) {
		scancode = SDL_SCANCODE_R;
	}
	if (keyState[SDL_SCANCODE_A]) {
		scancode = SDL_SCANCODE_A;
	}
	if (keyState[SDL_SCANCODE_S]) {
		scancode = SDL_SCANCODE_S;
	}
	if (keyState[SDL_SCANCODE_D]) {
		scancode = SDL_SCANCODE_D;
	}
	if (keyState[SDL_SCANCODE_F]) {
		scancode = SDL_SCANCODE_F;
	}
	if (keyState[SDL_SCANCODE_Z]) {
		scancode = SDL_SCANCODE_Z;
	}
	if (keyState[SDL_SCANCODE_X]) {
		scancode = SDL_SCANCODE_X;
	}
	if (keyState[SDL_SCANCODE_C]) {
		scancode = SDL_SCANCODE_C;
	}
	if (keyState[SDL_SCANCODE_V]) {
		scancode = SDL_SCANCODE_V;
	}

	return scancode;
}
