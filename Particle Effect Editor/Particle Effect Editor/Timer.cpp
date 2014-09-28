#include "Timer.h"

//TODO: make high precision timer

Timer::Timer(bool started) {
	started ? Start() : _running = false;
}

void Timer::Start() { 
	_running = true;
	_startMs = SDL_GetTicks();
}

Uint32 Timer::Stop() { 
	Uint32 msElapsed = MsElapsed();
	_running = false;
	return msElapsed;
}        

Uint32 Timer::Restart() { 
	Uint32 msElapsed = MsElapsed();
	Start();
	return msElapsed;
}     
Uint32 Timer::MsElapsed() const {
	return _running ? SDL_GetTicks() - _startMs : -1; //-1 underflows to highest possible uint
} 

bool Timer::Running() const { 
	return _running;
}
