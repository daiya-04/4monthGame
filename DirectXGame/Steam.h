#pragma once
#include "Particle.h"
#include "Vec2.h"
#include "RandomEngine/RandomEngine.h"

#include <memory>
#include <list>


class Steam {
public:

	void Init(const Vector2& centerPos,const Vector2& range);

	void Update();

	void Draw(const Camera& camera);

	void SetCenter(const Vector2& centerPos) { emitter_.translate_ = centerPos; }


	void SetEmitCount(uint32_t count) { emitter_.count_ = count; }

private:

	std::list<Particle::ParticleData> Emit(const Particle::Emitter& emitter);

	Particle::ParticleData MakeNewParticle(const Vector2& translate);

private:

	const float deltaTime_ = 1.0f / 60.0f;

	uint32_t particleTex_ = 0;

	std::unique_ptr<Particle> particle_;
	std::list<Particle::ParticleData> datas_;
	Particle::Emitter emitter_;

	Vector2 range_{};

};

