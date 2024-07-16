#include "HitEffect.h"
#include "RandomEngine/RandomEngine.h"
void HitEffect::Initialize(const Vector2& postition) {
	position_ = postition;
	position_.y += RandomEngine::GetRandom(-4.0f, 4.0f);
	position_.x += RandomEngine::GetRandom(-4.0f, 4.0f);
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	aliveTime_ = 20;
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	
	rotate_ = RandomEngine::GetRandom(0.0f, 3.141592f * 2.0f);
}

void HitEffect::Update() {
	aliveTime_--;
	if (aliveTime_ <= 0) {
		isAlive_ = false;
	}
	color_.w = float(aliveTime_) / float(aliveMax_);
}


void HitEffect::Draw() {
	BlockTextureManager::GetInstance()->AppendHitEffect(position_,rotate_, color_);
}
