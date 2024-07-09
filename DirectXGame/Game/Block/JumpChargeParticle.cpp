#include "JumpChargeParticle.h"
#include "RandomEngine/RandomEngine.h"
#include "Matrix44.h"
static Matrix4x4 smt;
int32_t JumpChargeParticle::response;
void JumpChargeParticle::Initialize(const Vector2& postition, int32_t type) {
	position_ = postition;
	offset_.x = RandomEngine::GetRandom(-1.0f, 1.0f);
	offset_.y = RandomEngine::GetRandom(-1.0f, 1.0f);
	offset_ = offset_.Normalize();
	offset_ *= 64.0f;
	type_ = type;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	aliveTime_ = 15;
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	response = 5;

	//rotate_ = DirectionToDirection(Vector3{ offset_.x,offset_.y,1.0f }.Normalize(), Vector3{0,0,0}).m[2][0];
	
	rotate_ = (0);
}

void JumpChargeParticle::Update(const Vector2& targetPosition) {
	aliveTime_--;
	if (aliveTime_ <= 0) {
		isAlive_ = false;
	}
	color_.w = float(aliveTime_) / float(aliveMax_);
	float t = float(aliveTime_+6) / float(aliveMax_);
	position_ = targetPosition + t* offset_;
}


void JumpChargeParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendJumpChargeParticle(position_,rotate_, type_, color_);
}
