#pragma once

#include "GUIWidget.h"
#include "InteractableRect.h"
#include "SDL_ttf.h"
#include <string>
#include <functional>
#include <vector>

class Button : public GUIWidget {
private:
	InteractableRect _interactable;
	int _labelX, _labelY;
	SDL_Texture *_renderedLabel;
	std::vector<std::function<void()>*> _onClickEvents; //use pointers so they're equality comparable (RemoveEvent)

public:
	Button(const SDL_Rect &rect, const SDL_Color &colorNormal, const SDL_Color &colorHighlighted, const SDL_Color &colorPressed,
		   TTF_Font *font, const SDL_Color &fontColor, const std::string &label, std::function<void()> *onClick);
	virtual ~Button() override;

	virtual void Update(int mouseX, int mouseY, bool pressed) override;
	virtual void Draw() override;

	void AddOnClickEvent(std::function<void()> *func);
	void RemoveOnClickEvent(std::function<void()> *func);

	void operator+=(std::function<void()> *func) { AddOnClickEvent(func); }
	void operator-=(std::function<void()> *func) { RemoveOnClickEvent(func); }
};