#include "StarParticle.h"
#include "RandomEngine/RandomEngine.h"
int32_t StarParticle::response;
void StarParticle::Initialize(const Vector2& postition) {
	position_ = postition;
	position_.y += RandomEngine::GetRandom(-64.0f, 64.0f);
	position_.x += RandomEngine::GetRandom(-64.0f, 64.0f);
	color_ = {1.0f,1.0f,1.0f,1.0f};
	aliveTime_ = int32_t(RandomEngine::GetRandom(12.0f, 30.0f));
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	response = int32_t(RandomEngine::GetRandom(10.0f, 40.0f));
}

void StarParticle::Update() {
	aliveTime_--;
	if (aliveTime_ <= 0) {
		isAlive_ = false;
	}
	color_.w =   float(aliveTime_) / float(aliveMax_);
}


void StarParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendStarParticle(position_,color_ );
}
