#pragma once

#include "SDL_ttf.h"
#include <vector>
#include <functional>

class GUIWidget;
class ParticleEmitter;

class ParticleEditorGUI {
private:
	std::vector<GUIWidget*> _widgets;
	ParticleEmitter *_emitter;
	TTF_Font *_font; //TODO: replace with bitmap font 
	SDL_Rect _backgroundRect;
	SDL_Color _backgroundColor;
	int _posX, _posY;

	int PlaceBelowPrevious(int startY);
	
	void CreateButton(const std::string &label, std::function<void()> *onClick);
	template<class T>
	void CreateSlider(const std::string &label, T &bindVariable, T min, T max);
	template<class T>
	void CreateSlider(const std::string &label, const std::function<T()> &getter, const std::function<void(T)> &setter, T min, T max);

public:
	ParticleEditorGUI(int posX, int posY, int screenHeight);
	~ParticleEditorGUI();

	void Update(double dt, int mouseX, int mouseY, bool pressed);
	void Draw(double msPerFrame);
};

