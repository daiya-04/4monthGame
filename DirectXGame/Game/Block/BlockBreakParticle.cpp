#include "BlockBreakParticle.h"
#include "RandomEngine/RandomEngine.h"
void BlockBreakParticle::Initialize(const Vector2& postition, uint32_t blockType) {
	position_ = postition;
	blockType_ = blockType;
	velocity_.y = RandomEngine::GetRandom(-2.0f, -1.0f);
	velocity_.x = RandomEngine::GetRandom(-2.0f,2.0f);
	//velocity_ = velocity_.Normal();
	velocity_ *= 2.0f;
	aliveTime_ = 30;
	isAlive_ = true;
}

void BlockBreakParticle::Update() {
	velocity_ += Vector2{0.0f,0.4f};
	position_ += velocity_;
	aliveTime_--;
	if (aliveTime_<=0) {
		isAlive_ = false;
	}
}


void BlockBreakParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendParticle(position_,blockType_);
}
