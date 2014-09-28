#include "Particle.h"
#include "Window.h"

void Particle::Update(double dt) {
	_velocity += _acceleration * dt;
	_centre += _velocity * dt;
	_scale += _scalingVelocity * dt; 
	_angle += _angularVelocity * dt;
	_TTL -= dt;
}

//It is your responsibility to check if IsAlive() first.
void Particle::Draw(SDL_Texture *texture, int texWidth, int texHeight) {
	Window::Draw(texture, _centre.X - (texWidth * _scale/2), _centre.Y - (texWidth * _scale/2), _scale, _scale, nullptr, _angle, 0, 0, _flip); 
}