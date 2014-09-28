#pragma once

#include "ParticleEmitter.h"

//Delete everything coming out of this factory.
class ParticleEmitterFactory {
public:
	//Fire
	static ParticleEmitter* createFire(Vec2 centre);
	static ParticleEmitter* createFireIntense(Vec2 centre);
	static ParticleEmitter* createFireExhaust(Vec2 centre);
	static ParticleEmitter* createFireExhaustSlow(Vec2 centre);

	//Smoke
	static ParticleEmitter* createSmoke(Vec2 centre);
	static ParticleEmitter* createSmokeIntense(Vec2 centre);
	static ParticleEmitter* createSmokeRising(Vec2 centre);

	//Explosions
	static ParticleEmitter* createExplosion(Vec2 centre);

	static ParticleEmitter* createTestEmitter(Vec2 centre);
};

