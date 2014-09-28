#include "Window.h"
#include "SDL_image.h"
#include "MathFunc.h"
#include <time.h>

using namespace std;

SDL_Window* Window::_window = nullptr;
SDL_Renderer* Window::_renderer = nullptr;

void Window::Init(const string &title, int width, int height) {
	 //Init SDL
	if (SDL_Init(SDL_INIT_EVERYTHING) == -1) //TODO: init less http://wiki.libsdl.org/moin.fcg/SDL_Init
		throw runtime_error("SDL Init Failed: " + string(SDL_GetError()));
	if (TTF_Init() == -1)
		throw runtime_error("TTF Init Failed"  + string(TTF_GetError()));
	 
	//Create window
	_window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, width, height, SDL_WINDOW_SHOWN);
	if (_window == nullptr)
		throw runtime_error("Failed to create window");
	 
	//Init renderer
	_renderer = SDL_CreateRenderer(_window, -1, SDL_RENDERER_ACCELERATED); // | SDL_RENDERER_PRESENTVSYNC);
	if (_renderer == nullptr)
		throw runtime_error("Failed to create renderer");

	//Seed random number generator
	srand((unsigned int)time(NULL));
}

void Window::Quit() {
	SDL_DestroyWindow(_window);
	SDL_DestroyRenderer(_renderer);
	TTF_Quit();
	SDL_Quit();
}

//Create SDL_Texture suitable for use as a render target. Use Window::SetRenderTarget to use.  
SDL_Texture* Window::CreateRenderTarget() {
	SDL_Rect window = AsRect();
	return CreateRenderTarget(window.w, window.h);
}
SDL_Texture* Window::CreateRenderTarget(int width, int height) {
	//SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0xff000000, 0x00ff0000, 0x0000ff00, 0x000000ff);
	//SDL_Surface *surface = SDL_CreateRGBSurface(0, width, height, 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000);
	
	//SDL_Texture *texture = SDL_CreateTextureFromSurface(_renderer, surface);
	//return texture;
	return SDL_CreateTexture(_renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, width, height);
}

/*Loads an image and returns it as a texture
* @param fileName The image file to load, relative to compiled program
* @return SDL_Texture* to the loaded texture. DELETE WITH  SDL_DestroyTexture() */
SDL_Texture* Window::LoadImage(const string &fileName) {
	SDL_Texture *tex = IMG_LoadTexture(_renderer, fileName.c_str());
	if (tex == nullptr)
		throw runtime_error("Failed to load image: " + fileName + IMG_GetError());
	return tex;
}

/*Load a font file. If you're going to draw text repeatedly, use this function and save the 
* result rather than continuously calling RenderText with just a font name (it's faster).
* @param fontPath The font to load, relative to compiled program
* @param fontSize The size to load the font at 
* @return A TTF_Font* to loaded font. DELETE WITH TTF_CloseFont() */
TTF_Font* Window::LoadFont(const string &fontPath, int size) {
	TTF_Font *font = TTF_OpenFont(fontPath.c_str(), size);
	if (font == nullptr)
		throw runtime_error("Failed to load font: " + fontPath + TTF_GetError());
	return font;
}

/*Generate a texture containing the text specified in the font specified
* @param text The text to display
* @param font The font to render the text with
* @param color The color of the rendered font
* @return An SDL_Texture* to the rendered message */
SDL_Texture* Window::RenderText(const string &text, TTF_Font *font, const SDL_Color &color) {
	SDL_Surface *surface = TTF_RenderText_Blended(font, text.c_str(), color);
	SDL_Texture *tex = SDL_CreateTextureFromSurface(_renderer, surface);
	SDL_FreeSurface(surface);
	return tex;
}

void Window::Clear(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha) {
	SDL_SetRenderDrawBlendMode(_renderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(_renderer, red, green, blue, alpha);
	SDL_RenderClear(_renderer);
}

void Window::Refresh() {
	SDL_RenderPresent(_renderer);
}

/*Draw a SDL_Texture to the screen.
* @param tex The SDL_Texture to draw
* @param pos_x Draw position, x coordinate
* @param pos_y Draw position, y coordinate
* @param scale_x Draw scale (1 is normal, 0 is infinitely small, 2 is twice normal)
* @param scale_y Draw scale (1 is normal, 0 is infinitely small, 2 is twice normal)
* @param clip The clip to apply to the image, if desired
* @param angle The rotation angle to apply to the texture, default is 0
* @param centre_x The x coordinate of the centre (rotates around), relative to actual tex centre
* @param centre_y The y coordinate of the centre (rotates around), relative to actual tex centre
* @param flip The flip to apply to the image, default is none */
void Window::Draw(SDL_Texture *tex, int pos_x, int pos_y, float scale_x, float scale_y, SDL_Rect *clip, float angle, 
				 int centre_x, int centre_y, SDL_RendererFlip flip) {
	//Position and scale dstRect
	SDL_Rect dstRect = {pos_x, pos_y};
	if (clip == nullptr)
		SDL_QueryTexture(tex, nullptr, nullptr, &dstRect.w, &dstRect.h);
	else {
		dstRect.w = clip->w;
		dstRect.h = clip->h;
	}
	dstRect.w = roundi(dstRect.w * scale_x);
	dstRect.h = roundi(dstRect.h * scale_y);
	//Convert centre pos from relative to object centre to screen space
	SDL_Point centre = {roundi((centre_x * scale_x) + dstRect.w / 2.f),
						roundi((centre_y * scale_y) + dstRect.h / 2.f)};
	//Draw the texture
	SDL_RenderCopyEx(_renderer, tex, clip, &dstRect, angle, &centre, flip);
}

//Draws text straight to window. Useful when you don't want to reuse the texture generated by RenderText.
void Window::Draw(const string &text, TTF_Font *font, const SDL_Color &color, int x, int y) {
	SDL_Texture *renderedText = RenderText(text, font, color);
	Draw(renderedText, x, y);
	SDL_DestroyTexture(renderedText);
}

//Get the window as a SDL_Rect {x, y, w, h}
SDL_Rect Window::AsRect() {
	SDL_Rect toReturn;
	SDL_GetWindowSize(_window, &toReturn.w, &toReturn.h);
	return toReturn;
}