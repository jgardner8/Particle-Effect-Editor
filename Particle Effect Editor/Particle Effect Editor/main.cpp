#include <stdexcept>
#include <iostream>
#include "Window.h"
#include "Timer.h"
#include "ParticleEditorGUI.h"

//#include "vld.h" //check for memory leaks on debug run

#define GAME_NAME "Particle Effect Editor"
#define SCREEN_WIDTH 1000 
#define SCREEN_HEIGHT 700
#define GUI_POS_X SCREEN_WIDTH-200
#define GUI_POS_Y 0

using namespace std;

int main(int argc, char** argv) {	
	//Init
	try {
		Window::Init(GAME_NAME, SCREEN_WIDTH, SCREEN_HEIGHT);
	} catch (const runtime_error &e){
		cerr << e.what() << std::endl;
		Window::Quit();
		return -1;
	}

	//Create timers
	Timer frameDeltaCalculator;
	frameDeltaCalculator.Start();

	//Create editor
	auto editor = new ParticleEditorGUI(GUI_POS_X, GUI_POS_Y, SCREEN_HEIGHT); //destructor must be called before Window::Quit()

	//Game Loop
	SDL_Event event;
	bool quit = false;
	int mouseX, mouseY;
	bool mouseLeftPressed = false;
	while (!quit) {
		double dt = frameDeltaCalculator.Restart() / 1000.0; //measured in seconds

		//Process events
		while (SDL_PollEvent(&event)) {
			switch (event.type) {
			case SDL_QUIT:
				quit = true;
				break;
			case SDL_MOUSEBUTTONDOWN:
			case SDL_MOUSEBUTTONUP:
				if (event.button.button == SDL_BUTTON_LEFT)
					mouseLeftPressed = event.type == SDL_MOUSEBUTTONDOWN;
				break;
			}
		}

		//Update state
		SDL_GetMouseState(&mouseX, &mouseY);
		editor->Update(dt, mouseX, mouseY, mouseLeftPressed);

		//Render
		Window::Clear();
		//Window::Clear(128, 128, 255);

		editor->Draw(dt*1000);

		Window::Refresh();
	}

	//Cleanup
	delete editor;
	Window::Quit();
	return 0;
}