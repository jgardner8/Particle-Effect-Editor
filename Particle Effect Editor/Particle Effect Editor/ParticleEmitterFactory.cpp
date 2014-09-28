#include "ParticleEmitterFactory.h"
#include "Window.h"

//Fire
ParticleEmitter* ParticleEmitterFactory::createFire(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/flame.png");
	return new ParticleEmitter(tex, centre, 100, 0.1f, 0.20f, 0, 150, 360, 720, 0.25f, 1, 0, 2, Vec2(0, 0), 360, 0, SDL_BLENDMODE_ADD, 255, 60, 15, 255, -1); 
}
ParticleEmitter* ParticleEmitterFactory::createFireIntense(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/flame.png");
	return new ParticleEmitter(tex, centre, 250, 0.1f, 0.20f, 0, 200, 360, 720, 0.25f, 1, 0, 2, Vec2(0, 0), 360, 0, SDL_BLENDMODE_ADD, 255, 80, 30, 255, 0); 
}
ParticleEmitter* ParticleEmitterFactory::createFireExhaust(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/flame.png");
	return new ParticleEmitter(tex, centre, 350, 0.1f, 0.3f, 100, 400, 360, 720, 0.25f, 1, 0, 2, Vec2(0, 0), 45, 90, SDL_BLENDMODE_ADD, 255, 80, 30, 255, 0); 
}
ParticleEmitter* ParticleEmitterFactory::createFireExhaustSlow(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/flame.png");
	return new ParticleEmitter(tex, centre, 175, 0.2f, 0.6f, 50, 200, 180, 360, 0.25f, 1, 0, 1, Vec2(0, 0), 45, 90, SDL_BLENDMODE_ADD, 255, 80, 30, 255, 0); 
}

//Smoke
ParticleEmitter* ParticleEmitterFactory::createSmoke(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/cloud.png");
	return new ParticleEmitter(tex, centre, 125, 0.8f, 1, 0, 50, 10, 90, 0.01f, 0.05f, 0.2f, 0.3f, Vec2(0, 0), 360, 0, SDL_BLENDMODE_BLEND, 200, 200, 200, 150, 0);
}
ParticleEmitter* ParticleEmitterFactory::createSmokeIntense(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/cloud.png");
	return new ParticleEmitter(tex, centre, 150, 1, 1.5f, 0, 50, 10, 90, 0.01f, 0.05f, 0.5f, 0.8f, Vec2(0, 0), 360, 0, SDL_BLENDMODE_BLEND, 200, 200, 200, 145, 0);
}
ParticleEmitter* ParticleEmitterFactory::createSmokeRising(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/cloud.png");
	return new ParticleEmitter(tex, centre, 50, 3, 6, 0, 0, 10, 90, 0.005f, 0.01f, 0.1f, 0.2f, Vec2(0, -60), 360, 0, SDL_BLENDMODE_BLEND, 200, 200, 200, 50, 0, 15);
}
 
//Explosion
ParticleEmitter* ParticleEmitterFactory::createExplosion(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/flame.png");
	return new ParticleEmitter(tex, centre, 8200, 0.5f, 0.5f, 0, 200, 360, 720, 0.25f, 1, 0.4f, 0.7f, Vec2(0, 0), 360, 0, SDL_BLENDMODE_ADD, 255, 80, 30, 255, 0.3f, 0.05f); 	
}

//Free to modify to test new features
ParticleEmitter* ParticleEmitterFactory::createTestEmitter(Vec2 centre) {
	SDL_Texture *tex = Window::LoadImage("img/cloud.png");
	return new ParticleEmitter(tex, centre, 1, 3, 3, 0, 50, 10, 90, 0.01f, 0.05f, 0.2f, 0.5f, Vec2(0, 0), 360, 0, SDL_BLENDMODE_BLEND, 200, 200, 200, 128, 0.8f); 
}