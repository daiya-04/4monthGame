#include "Steam.h"
#include "TextureManager.h"

void Steam::Init(const Vector2& centerPos, const Vector2& range) {

	particleTex_ = TextureManager::Load("steam.png");

	particle_.reset(GPUParticle::Create(particleTex_, 2000));
	particle_->emitter_.translate = Vector2(640.0f, 650.0f);
	particle_->emitter_.size = range;
	particle_->emitter_.count = 30;
	particle_->emitter_.frequency = 0.1f;

}

void Steam::Update() {

	particle_->Update();

}

void Steam::Draw(const Camera& camera) {

	particle_->Draw(camera);

}

