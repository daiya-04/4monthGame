#pragma once
#include "GPUParticle.h"
#include "Vec2.h"
#include "RandomEngine/RandomEngine.h"

#include <memory>
#include <list>


class Steam {
public:

	void Init(const Vector2& centerPos,const Vector2& range);

	void Update();

	void Draw(const Camera& camera);

	void SetEmiiterPos(const Vector2& pos) { particle_->emitter_.translate = pos; }

private:

	const float deltaTime_ = 1.0f / 60.0f;

	uint32_t particleTex_ = 0;

	std::unique_ptr<GPUParticle> particle_;

};

