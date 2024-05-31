#include "StarParticle.h"
#include "RandomEngine/RandomEngine.h"
int32_t StarParticle::response;
void StarParticle::Initialize(const Vector2& postition, int32_t type) {
	position_ = postition;
	blockType_ = type;
	color_ = {1.0f,1.0f,1.0f,1.0f};
	aliveTime_ = 10;
	if (type > 0) {
		position_.y += RandomEngine::GetRandom(-64.0f, 64.0f);
		position_.x += RandomEngine::GetRandom(-64.0f, 64.0f);
		aliveTime_ = int32_t(RandomEngine::GetRandom(25.0f, 30.0f));
	}
	else {
		position_.y += RandomEngine::GetRandom(-32.0f, 32.0f);
		position_.x += RandomEngine::GetRandom(-32.0f, 32.0f);

	}
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	if (type>=0) {
		response = int32_t(RandomEngine::GetRandom(10.0f, 30.0f));
	}
	else {
		response = int32_t(RandomEngine::GetRandom(5.0f, 8.0f));
	}

	switch (blockType_)
	{
	case -1://r
		color_ = { 1.0f,0.6f,0.6f,1.0f };
		break;
	case -2://g
		color_ = { 0.6f,1.0f,0.6f,1.0f };
		break;
	case -3://b
		color_ = { 0.6f,0.6f,1.0f,1.0f };
		break;
	default:
		break;
	}
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

void StarParticle::DrawUI() {
	BlockTextureManager::GetInstance()->AppendStarParticleUI(position_, color_);
}
