#include "WallKickEffect.h"
#include "RandomEngine/RandomEngine.h"
int32_t WallKickEffect::response;
void WallKickEffect::Initialize(const Vector2& postition, int32_t type) {
	position_ = postition;
	type_ = type;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	aliveTime_ = 10;
	
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	
}

void WallKickEffect::Update() {
	aliveTime_--;
	if (aliveTime_ <= 0) {
		isAlive_ = false;
	}
	color_.w = float(aliveTime_) / float(aliveMax_);
}


void WallKickEffect::Draw() {
	BlockTextureManager::GetInstance()->AppendWallKickEffect(position_, type_,color_);
}
