#include "Button.h"
#include "SDL_ttf.h"
#include <algorithm>
#include "Window.h"

using namespace std;

Button::Button(const SDL_Rect &rect, const SDL_Color &colorNormal, const SDL_Color &colorHighlighted, const SDL_Color &colorPressed,
			   TTF_Font *font, const SDL_Color &fontColor, const string &label, function<void()> *onClick) 
			   : _interactable(rect, colorNormal, colorHighlighted, colorPressed) {	
	//Render label. Reuse previously rendered text in future.
	int labelW, labelH;
	TTF_SizeText(font, label.c_str(), &labelW, &labelH);
	_labelX = rect.x + (rect.w - labelW)/2;
	_labelY = rect.y + (rect.h - labelH)/2;
	_renderedLabel = Window::RenderText(label, font, fontColor);

	*this += onClick;
}

Button::~Button() {
	SDL_DestroyTexture(_renderedLabel);

	for (auto func : _onClickEvents)
		delete func;
	_onClickEvents.clear();
}

void Button::Update(int mouseX, int mouseY, bool pressed)  { 
	//if was pressed and is now released, trigger _onClickEvents
	if (!pressed 
	 && _interactable.State() == InteractableState::PRESSED
	 && _interactable.IsAt(mouseX, mouseY)) {
		 for (auto funcPtr : _onClickEvents)
			 (*funcPtr)();
	}

	_interactable.Update(mouseX, mouseY, pressed);
}

void Button::Draw() { 
	_interactable.Draw();
	Window::Draw(_renderedLabel, _labelX, _labelY);
}

void Button::AddOnClickEvent(std::function<void()> *func) { 
	_onClickEvents.push_back(func);
}

void Button::RemoveOnClickEvent(std::function<void()> *func) { 
	_onClickEvents.erase( remove(begin(_onClickEvents), end(_onClickEvents), func) , end(_onClickEvents));
	delete func;
}