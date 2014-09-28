#pragma once

//Used to relate sliders and buttons by subtype
class GUIWidget { 
public:
	virtual ~GUIWidget() { }
	virtual void Update(int mouseX, int mouseY, bool pressed) = 0;
	virtual void Draw() = 0;
};