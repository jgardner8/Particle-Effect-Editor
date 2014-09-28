#pragma once

#include "SDL.h"
#include "SDL_ttf.h"
#include <string>

//A facade on top of the SDL2 API.
class Window {
private:
	static SDL_Window *_window;
	static SDL_Renderer *_renderer;

public:
	static void Init(const std::string &title, int width, int height);
	static void Quit();

	static SDL_Texture* CreateRenderTarget();
	static SDL_Texture* CreateRenderTarget(int width, int height);
	static SDL_Texture* LoadImage(const std::string &path);
	static TTF_Font* LoadFont(const std::string &fontPath, int size);
	static SDL_Texture* RenderText(const std::string &text, TTF_Font *font, const SDL_Color &color);
	
	static void Clear(Uint8 red=0, Uint8 green=0, Uint8 blue=0, Uint8 alpha=255);
	static void Refresh();
	static void Draw(SDL_Texture *tex, int pos_x, int pos_y, float scale_x=1, float scale_y=1, SDL_Rect *clip=nullptr, 
	                 float angle=0.0, int centre_x=0, int centre_y=0, SDL_RendererFlip flip=SDL_FLIP_NONE);
	static void Draw(const std::string &text, TTF_Font *font, const SDL_Color &color, int x, int y);

	static SDL_Rect AsRect();
	static SDL_Renderer* Renderer() { return _renderer; }
};