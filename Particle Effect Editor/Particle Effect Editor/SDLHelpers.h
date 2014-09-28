#pragma once

#include "SDL.h"

inline SDL_Rect Rect(int x, int y, int w, int h) {
	SDL_Rect r;
	r.x = x;
	r.y = y;
	r.w = w;
	r.h = h;
	return r;
}

inline SDL_Color Color(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
	SDL_Color c;
	c.r = r;
	c.g = g;
	c.b = b;
	c.a = a;
	return c;
}
