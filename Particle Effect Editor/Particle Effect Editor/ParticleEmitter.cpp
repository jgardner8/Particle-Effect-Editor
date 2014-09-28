#include "ParticleEmitter.h"
#include "MathFunc.h"
#include "Window.h"
#include "Units.h"
#include <iostream>

/*Creates a particle emitter with a pre-initialised pool of particles
* @param texture Used to draw all particles in this emitter. The emitter TAKES OWNERSHIP of the texture. DO NOT DELETE. 
* @param centre Position of emitter, stored as a centre
* @param emitPerSecond Number of particles to emit per second
* @param minTTL Minimum number of seconds particles will "survive"
* @param maxTTL Maximum number of seconds particles will "survive"
* @param minSpeed Minimum speed of each particle regardless of angle
* @param maxSpeed Maximum speed of each particle regardless of angle
* @param minAngularVelocity Minimum degrees/sec each particle will rotate (- or +)
* @param maxAngularVelocity Maximum degrees/sec each particle will rotate (- or +)
* @param particleAcceleration Amount of acceleration that particle is applied. Simulates wind and gravity.
* @param spreadConstraint Amount to constrict the direction that particles are emitted, as an angle. 360 means that particles can be emitted in all directions, whereas 0 indicates a straight line.
* @param emitterAngle The angle the emitter is facing. Only matters if spreadConstraint is < 360.
* @param blendMode Can choose between alpha, additive, modulative and no blending.
* @param red Amount to colour-shift the red channel by (255 being no shift, 0 being no red) 
* @param green Amount to colour-shift the green channel by (255 being no shift, 0 being no green) 
* @param blue Amount to colour-shift the blue channel by (255 being no shift, 0 being no blue) 
* @param alpha Amount of transparency, from 0 (fully transparent) to 255 (fully opaque). If fadeAtAge is not -1, this is the max possible alpha, from which fading will start.
* @param fadeParticlesAtAge The proportion of total life where particle will start to fade out, in range 0-1, where 0 is fade right away and 1 is fade at last possible moment. -1 = don't fade
* @param emitterTTL The amount of particles emitted is reduced as emitterTTL is approached, until the emitter is declared dead and can be deleted. -1 means live forever. */
ParticleEmitter::ParticleEmitter(SDL_Texture *texture, const Vec2 &centre, float emitPerSecond, float minTTL, float maxTTL, 
								 float minSpeed, float maxSpeed, float minAngularVelocity, float maxAngularVelocity, 
								 float minScale, float maxScale, float minScalingVelocity, float maxScalingVelocity, const Vec2 &particleAcceleration,
								 float spreadConstraint, float emitterAngle, SDL_BlendMode blendMode, Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha, 
								 float fadeParticlesAtAge, float emitterTTL) {
	_fractionalToEmit = 0;
	_timePassed = 0;

	Centre = centre;
	EmitPerSecond = _originalEmitPerSecond = emitPerSecond;
	MinTTL = minTTL;
	MaxTTL = maxTTL;
	MinSpeed = minSpeed;
	MaxSpeed = maxSpeed;
	MinAngularVelocity = minAngularVelocity;
	MaxAngularVelocity = maxAngularVelocity;
	MinScale = minScale;
	MaxScale = maxScale;
	MinScalingVelocity = minScalingVelocity;
	MaxScalingVelocity = maxScalingVelocity;
	ParticleAcceleration = particleAcceleration;
	SpreadConstraint = spreadConstraint;
	EmitterAngle = emitterAngle;
	FadeParticlesAtAge = fadeParticlesAtAge;
	EmitterTTL = emitterTTL;

	_red = red;
	_green = green;
	_blue = blue;
	StartAlpha = alpha;
	_blendMode = blendMode;
	_texture = nullptr; //avoid destruction
	Texture(texture);

	//If blendMode is additive (SDL_BLENDMODE_ADD), everything must be rendered to a texture
	//  and this texture then rendered to the existing scene using alpha blending. Otherwise
	//  only black or other really dark backgrounds look any good.
	if (blendMode == SDL_BLENDMODE_ADD) {
		_canvas = Window::CreateRenderTarget();
		SDL_SetTextureBlendMode(_canvas, SDL_BLENDMODE_BLEND);
		SDL_SetTextureAlphaMod(_canvas, 255);
		SDL_SetTextureColorMod(_canvas, 255, 255, 255);
	} else 
		_canvas = nullptr;
}

ParticleEmitter::~ParticleEmitter() {
	SDL_DestroyTexture(_texture);
	if (_canvas != nullptr)
		SDL_DestroyTexture(_canvas);
}

void ParticleEmitter::Update(double dt) {
	//Update number of particles to emit
	if (EmitterTTL > 0) {
		_timePassed += (float)dt;
		EmitPerSecond = (_timePassed < EmitterTTL)
			? (1 - (_timePassed / EmitterTTL)) * _originalEmitPerSecond
			: 0; //makes monitoring slightly easier, rather than having slightly off 0 values
	}

	//Calculate amount of particles to emit this frame
	double toEmit = EmitPerSecond * dt;
	_fractionalToEmit += toEmit - floor(toEmit);
	while (_fractionalToEmit > 1) {
		toEmit++;
		_fractionalToEmit--;
	}
	toEmit = floor(toEmit);

	//Update and emit particles (if is alive, update, otherwise use as empty object for new particle)
	int emitted = 0;
	for (int i = 0; i < PARTICLE_POOL_SIZE; i++) {
		if (_particles[i].IsAlive())
			_particles[i].Update(dt);
		else if (emitted < toEmit) {
			_particles[i]._centre = Centre;
			double moveAngle = (Random(EmitterAngle - SpreadConstraint/2, EmitterAngle + SpreadConstraint/2) + 90) * DEGTORAD; //angle 0 is right, 90 is up.
			_particles[i]._velocity = Vec2(sin(moveAngle), cos(moveAngle)) * Random(MinSpeed, MaxSpeed); //speed = hypotenuse, velX = opposite, velY = adjacent
			_particles[i]._acceleration = ParticleAcceleration;
			
			_particles[i]._angle = Random(0.f, 360.f);
			_particles[i]._angularVelocity = PosOrNeg(Random(MinAngularVelocity, MaxAngularVelocity));
			
			_particles[i]._scale = Random(MinScale, MaxScale);
			_particles[i]._scalingVelocity = Random(MinScalingVelocity, MaxScalingVelocity);
			
			_particles[i]._TTL = _particles[i]._startTTL = Random(MinTTL, MaxTTL);
			_particles[i]._flip = (SDL_RendererFlip)Random(0, 3);
			emitted++;
		}
	}
}

void ParticleEmitter::Draw() {
	//Set render target (if applicable)
	if (_canvas) {
		SDL_SetRenderTarget(Window::Renderer(), _canvas);
		Window::Clear(0, 0, 0, 0);
	}

	//Draw
	Uint8 alpha;
	float age; 
	for (int i = 0; i < PARTICLE_POOL_SIZE; i++) {
		if (_particles[i].IsAlive()) {
			//Calculate alpha
			if (FadeParticlesAtAge != -1) {
				age = 1 - (_particles[i]._TTL / _particles[i]._startTTL);
				alpha = (age > FadeParticlesAtAge) ? (Uint8)floor((1-age)/(1-FadeParticlesAtAge) * StartAlpha) : StartAlpha;
				SDL_SetTextureAlphaMod(_texture, alpha);
			}
			_particles[i].Draw(_texture, _texWidth, _texHeight);
		}	
	}

	//If render target was not window, copy render target to window
	if (_canvas) {
		SDL_SetRenderTarget(Window::Renderer(), nullptr); //set render target to default
		SDL_RenderCopy(Window::Renderer(), _canvas, nullptr, nullptr);
	}
}

bool ParticleEmitter::IsAlive() {
	return EmitterTTL < 0 || _timePassed < (EmitterTTL + MaxTTL);
}

void ParticleEmitter::Texture(SDL_Texture *val) {
	if (_texture)
		SDL_DestroyTexture(_texture);
	_texture = val;
	SDL_QueryTexture(_texture, nullptr, nullptr, &_texWidth, &_texHeight);
	SDL_SetTextureBlendMode(_texture, _blendMode);
	SDL_SetTextureAlphaMod(_texture, StartAlpha);
	UpdateTexColor();
}