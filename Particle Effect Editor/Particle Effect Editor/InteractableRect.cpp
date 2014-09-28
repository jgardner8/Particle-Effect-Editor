#include "InteractableRect.h"
#include "Window.h"

InteractableRect::InteractableRect(const SDL_Rect &rect, const SDL_Color &colorNormal, const SDL_Color &colorHighlighted, const SDL_Color &colorPressed) {
	_rect = rect;
	_colors[0] = colorNormal;
	_colors[1] = colorHighlighted;
	_colors[2] = colorPressed;
	_currentState = InteractableState::NORMAL;
}

bool InteractableRect::IsAt(int posX, int posY) {
	return posX >= _rect.x
		&& posY >= _rect.y
		&& posX < _rect.x + _rect.w
		&& posY < _rect.y + _rect.h;
}

void InteractableRect::Update(int mouseX, int mouseY, bool pressed) {
	_currentState = IsAt(mouseX, mouseY)
		? pressed 
			? InteractableState::PRESSED 
			: InteractableState::HIGHLIGHTED
		: _currentState = InteractableState::NORMAL;
}

void InteractableRect::Draw() {
	SDL_Color c = _colors[(int)_currentState];
	SDL_SetRenderDrawColor(Window::Renderer(), c.r, c.g, c.b, c.a); 
	SDL_RenderFillRect(Window::Renderer(), &_rect);
}