#pragma once

#include "GUIWidget.h"
#include "InteractableRect.h"
#include "MathFunc.h"
#include "SDL.h"
#include "SDL_ttf.h"
#include <string>
#include <sstream>
#include <functional>

//Intended types for T include double, float, int, etc. 
//Technically anything that can be assigned a number and overloads > and < can be used
template<class T>
class Slider : public GUIWidget {
private:
	static const int SLIDER_WIDTH = 15;

	//If user specifies a bindVariable instead of a getter and setter
	//  getters/setters are generated to interact with the bind variable.
	std::function<T()> _getter;
	std::function<void(T)> _setter;
	T _min, _max;

	SDL_Color _colorBackground;
	SDL_Rect _rect; //the background notch that the slider sits in

	InteractableRect _interactable; //the slider itself
	
	int _labelX, _labelY;
	SDL_Texture *_renderedLabel; //the text that labels the slider is pre-rendered
	TTF_Font *_font; //the current value of bindVariable is rendered each frame using this font
	SDL_Color _fontColor; 

	void Init(const SDL_Rect &rect, const SDL_Color &colorBackground, TTF_Font *font, const SDL_Color &fontColor, const std::string &label, 
	          const std::function<T()> &getter, const std::function<void(T)> &setter, T min, T max) {
		_getter = getter;
		_setter = setter;
		_min = min;
		_max = max;
		_colorBackground = colorBackground;
		_rect = rect;
		_font = font;
		_fontColor = fontColor;

		//Set interactable rect to current value of bindVariable
		double proportion = (_getter() - min) / (double)(max - min);
		_interactable.Rect(Rect((int)(proportion * (rect.w-SLIDER_WIDTH)) + rect.x, rect.y, SLIDER_WIDTH, rect.h));

		//Render label. Reuse previously rendered text in future.
		int labelW, labelH;
		TTF_SizeText(font, label.c_str(), &labelW, &labelH);
		_labelX = rect.x + (rect.w - labelW)/2;
		_labelY = rect.y + (rect.h - labelH)/2;
		_renderedLabel = Window::RenderText(label, font, fontColor);
	}

public:
	Slider(const SDL_Rect &rect, const SDL_Color &colorNormal, const SDL_Color &colorHighlighted, const SDL_Color &colorPressed, 
		   const SDL_Color &colorBackground, TTF_Font *font, const SDL_Color &fontColor, const std::string &label, 
		   T &bindVariable, T min, T max) 
	   	   : _interactable(Rect(0,0,0,0), colorNormal, colorHighlighted, colorPressed) { 
		auto getter = std::function<T()>([&bindVariable] () { return bindVariable; }); 
		auto setter = std::function<void(T)>([&bindVariable] (T val) { bindVariable = val; });
		Init(rect, colorBackground, font, fontColor, label, getter, setter, min, max);	
	}

	Slider(const SDL_Rect &rect, const SDL_Color &colorNormal, const SDL_Color &colorHighlighted, const SDL_Color &colorPressed, 
		   const SDL_Color &colorBackground, TTF_Font *font, const SDL_Color &fontColor, const std::string &label, 
		   const std::function<T()> &getter, const std::function<void(T)> &setter, T min, T max) 
		   : _interactable(Rect(0,0,0,0), colorNormal, colorHighlighted, colorPressed) {
		Init(rect, colorBackground, font, fontColor, label, getter, setter, min, max);
	}
	
	virtual ~Slider() override {
		SDL_DestroyTexture(_renderedLabel);
	}

	virtual void Update(int mouseX, int mouseY, bool pressed) override {
		if (_interactable.State() != InteractableState::PRESSED)
			_interactable.Update(mouseX, mouseY, pressed);
		else {
			//Moving your mouse outside the slider should not unpress it, unlike most interactables
			SDL_Rect slider = _interactable.Rect();
			_interactable.Update(slider.x, slider.y, pressed);
			//Move slider
			int newPosX = constrain(mouseX - SLIDER_WIDTH/2, _rect.x, _rect.x + _rect.w-slider.w);
			_interactable.Rect(Rect(newPosX, slider.y, slider.w, slider.h));
			//Update binded variable
			float proportion = (newPosX - _rect.x) / (float)(_rect.w - slider.w);
			_setter((proportion * (_max - _min)) + _min);
		}
	}

	virtual void Draw() override {
		//Draw background notch, that slider sits in
		SDL_SetRenderDrawColor(Window::Renderer(), _colorBackground.r, _colorBackground.g, _colorBackground.b, _colorBackground.a); 
		SDL_RenderFillRect(Window::Renderer(), &_rect);
		//Draw slider
		_interactable.Draw();
		//Draw labels
		//	Slider label
		Window::Draw(_renderedLabel, _labelX, _labelY);
		//	Current value of _bindVariable
		stringstream currentValue;
		T val = _getter();
		currentValue << (val - (int)val == 0 ? (int)val : val); //hack to make Uint8s work. (floor breaks the linker here when in release mode. Casting to int serves the same purpose)

		SDL_Rect slider = _interactable.Rect();
		SDL_Rect label;
		TTF_SizeText(_font, currentValue.str().c_str(), &label.w, &label.h);
		label.x = slider.x + (slider.w - label.w)/2;
		label.y = slider.y + (slider.h - label.h)/2;
		Window::Draw(currentValue.str(), _font, _fontColor, label.x, label.y + slider.h/2 + label.h/2);
	}
};