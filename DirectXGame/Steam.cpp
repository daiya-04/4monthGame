#include "Steam.h"
#include "TextureManager.h"

void Steam::Init(const Vector2& centerPos, const Vector2& range) {

	particleTex_ = TextureManager::Load("steam.png");

	particle_.reset(Particle::Create(particleTex_, 100));

	range_ = range;

	emitter_.count_ = 2;
	emitter_.frequency_ = 0.4f;
	emitter_.translate_ = centerPos;

}

void Steam::Update() {

	emitter_.frequencyTime_ += deltaTime_;
	if (emitter_.frequencyTime_ >= emitter_.frequency_) {
		datas_.splice(datas_.end(), Emit(emitter_));
		emitter_.frequencyTime_ = 0.0f;
	}

	for (std::list<Particle::ParticleData>::iterator itPar = datas_.begin(); itPar != datas_.end(); itPar++) {
		(*itPar).pos_ += (*itPar).velocity_;
		(*itPar).currentTime_ += deltaTime_;
	}

}

void Steam::Draw(const Camera& camera) {

	particle_->Draw(datas_, camera);

}

std::list<Particle::ParticleData> Steam::Emit(const Particle::Emitter& emitter) {
	std::list<Particle::ParticleData> particles;
	for (uint32_t count = 0; count < emitter.count_; count++) {
		particles.push_back(MakeNewParticle(emitter.translate_));
	}

	return particles;
}

Particle::ParticleData Steam::MakeNewParticle(const Vector2& translate) {

	Particle::ParticleData particle;

	//particle.worldTransform_.translation_ = { /*distPos(randomEngine),distPos(randomEngine) ,distPos(randomEngine)*/ };
	particle.pos_ = { translate.x + RandomEngine::GetRandom(-range_.x,range_.x),translate.y + RandomEngine::GetRandom(-range_.y,range_.y) };
	particle.velocity_ = { 0.0f, RandomEngine::GetRandom(-3.0f,-1.0f) };
	particle.color_ = { 1.0f, 1.0f, 1.0f, 1.0f };
	particle.lifeTime_ = RandomEngine::GetRandom(1.0f, 4.0f);
	particle.currentTime_ = 0.0f;


	return particle;

}
