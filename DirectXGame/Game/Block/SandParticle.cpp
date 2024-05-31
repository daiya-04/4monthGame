#include "SandParticle.h"
#include "RandomEngine/RandomEngine.h"
int32_t SandParticle::response;
void SandParticle::Initialize(const Vector2& postition, int32_t type) {
	position_ = postition;
	position_.y += RandomEngine::GetRandom(-2.0f, 2.0f);
	position_.x += RandomEngine::GetRandom(-2.0f, 2.0f);
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	aliveTime_ = 20;
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	
	response = 0;
}

void SandParticle::Update() {
	aliveTime_--;
	if (aliveTime_ <= 0) {
		isAlive_ = false;
	}
	color_.w = float(aliveTime_) / float(aliveMax_);
}


void SandParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendSandParticle(position_, color_);
}
