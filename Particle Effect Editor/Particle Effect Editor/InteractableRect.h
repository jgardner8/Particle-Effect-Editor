#pragma once

#include "SDL.h"

enum class InteractableState { NORMAL, HIGHLIGHTED, PRESSED };

//A rectangle that changes colour as the mouse interacts with it. 
//Various GUIWidgets are composed of one of these, among other things. 
class InteractableRect {
private:
	InteractableState _currentState;
	SDL_Color _colors[3]; //use _currentState as an index
	SDL_Rect _rect;

public:
	InteractableRect(const SDL_Rect &rect, const SDL_Color &colorNormal, const SDL_Color &colorHighlighted, const SDL_Color &colorPressed);

	void Update(int mouseX, int mouseY, bool pressed);
	void Draw();

	bool IsAt(int posX, int posY);
	InteractableState State() { return _currentState; }
	SDL_Rect Rect() { return _rect; }
	void Rect(const SDL_Rect &r) { _rect = r; }
};

