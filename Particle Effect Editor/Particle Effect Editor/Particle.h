#pragma once

#include "SDL.h"
#include "Vec2.h"

class Particle {
friend class ParticleEmitter;
private:
	Vec2 _centre; //position, stored as a centre for easy scaling/particle emission
	Vec2 _velocity; //velocity
	Vec2 _acceleration; //accelerates particles to simulate wind/gravity
	float _angle, _angularVelocity; //angle, speed of rotation
	float _scale, _scalingVelocity; //scale, speed of growing/shrinking
	float _startTTL, _TTL; //Time to Live, dies when this reaches 0. _startTTL is used to work out age, for fading (see ParticleEmitter)
	SDL_RendererFlip _flip;

public:
	inline bool IsAlive() { return _TTL > 0 && _scale > 0; } //often used in object pools, so you need to be able to deactivate particles and reuse them later
	void Update(double dt);
	void Draw(SDL_Texture *texture, int texWidth, int texHeight); //particle emitters store the texture to avoid having memory management nightmares.
																  //Considered using a std::shared_ptr, but decided it was unnecessary overhead.
};


 