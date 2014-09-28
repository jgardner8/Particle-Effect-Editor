#pragma once

#include "SDL.h"
#include "Particle.h"
#include <cstdlib>

//Amount of particle objects allocated per emitter. 
//400 is around optimal for a release, but the editor of course must support more.
//TODO: set as low as possible before releasing game.
#define PARTICLE_POOL_SIZE 10000 

class ParticleEmitter {
friend class ParticleEditorGUI;
private:
	Particle _particles[PARTICLE_POOL_SIZE];
	SDL_Texture *_canvas; //a render texture used to correctly blend additive-blended particles with the scene.
	SDL_Texture *_texture; //used for all particles spawned from this emitter. Flyweight pattern. 
	int _texWidth, _texHeight; //used to place particles by centre
	double _fractionalToEmit; //when a particle emitter has to emit a fractional amount of particles, 
	                        //it keeps the debt in here until it reaches a whole number, then emits it.
	float _originalEmitPerSecond, _timePassed; //used for emitterTTL
	
	Uint8 _red, _green, _blue; //not actually stored for drawing, but to allow owner to get them and adjust them for smooth fading.
	SDL_BlendMode _blendMode;

	void UpdateTexColor() { SDL_SetTextureColorMod(_texture, _red, _green, _blue); }

public:
	Vec2 Centre;
	float EmitPerSecond;
	float MinTTL, MaxTTL;
	float MinSpeed, MaxSpeed;
	float MinAngularVelocity, MaxAngularVelocity;
	float MinScale, MaxScale;
	float MinScalingVelocity, MaxScalingVelocity;
	Vec2 ParticleAcceleration;
	float SpreadConstraint;
	float EmitterAngle;
	Uint8 StartAlpha;
	float FadeParticlesAtAge;
	float EmitterTTL;

	ParticleEmitter(SDL_Texture *texture, const Vec2 &centre, float emitPerSecond, float minTTL, float maxTTL, 
					float minSpeed, float maxSpeed, float minAngularVelocity=0, float maxAngularVelocity=0, 
					float minScale=1, float maxScale=1, float minScalingVelocity=0, float maxScalingVelocity=0, 
					const Vec2 &particleAcceleration=Vec2(0,0), float spreadConstraint=360, float emitterAngle=0, SDL_BlendMode blendMode=SDL_BLENDMODE_BLEND, 
					Uint8 red=255, Uint8 green=255, Uint8 blue=255, Uint8 alpha=255, float fadeParticlesAtAge=0, float emitterTTL=-1);
	~ParticleEmitter();

	void Update(double dt);
	void Draw();


	//Properties
	bool IsAlive(); 

	Uint8 Red() { return _red; }
	Uint8 Green() { return _green; }
	Uint8 Blue() { return _blue; }
	void Red(Uint8 val) { _red = val; UpdateTexColor(); }
	void Green(Uint8 val) { _green = val; UpdateTexColor(); } 
	void Blue(Uint8 val) { _blue = val; UpdateTexColor(); }

	SDL_Texture* Texture() { return _texture; }
	void Texture(SDL_Texture *val);
	SDL_BlendMode BlendMode() { return _blendMode; }
	void BlendMode(SDL_BlendMode val) { _blendMode = val; SDL_SetTextureBlendMode(_texture, _blendMode); }
};