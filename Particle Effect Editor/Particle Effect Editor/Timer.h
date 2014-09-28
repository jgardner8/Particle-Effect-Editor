#pragma once

#include "SDL.h"

class Timer {
private:
	Uint32 _startMs;
	bool _running;

public:
	Timer(bool started=false);

	void Start();
	Uint32 Stop();            //returns milliseconds elapsed
	Uint32 Restart();         //returns milliseconds elapsed
	Uint32 MsElapsed() const; //returns milliseconds elapsed

	bool Running() const;
};