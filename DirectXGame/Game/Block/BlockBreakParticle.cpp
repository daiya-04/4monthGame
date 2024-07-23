#include "BlockBreakParticle.h"
#include "RandomEngine/RandomEngine.h"
void BlockBreakParticle::Initialize(const Vector2& postition, uint32_t blockType) {
	position_ = postition;
	blockType_ = blockType;
	texBase_ = {0,0};
	if (blockType == 2 || blockType == 3 || blockType == 4 || blockType == 5 || blockType == 6 || blockType == 7) {
		texBase_.x = RandomEngine::GetRandom(0.0f, 1.0f);
		if (texBase_.x >0.5f) {
			texBase_.x = 1.0f;
		}
		else {
			texBase_.x = 0.0f;
		}
		texBase_.y = RandomEngine::GetRandom(0.0f, 1.0f);
		if (texBase_.y > 0.5f) {
			texBase_.y = 1.0f;
		}
		else {
			texBase_.y = 0.0f;
		}
		texBase_ *= 32.0f;
	}
	velocity_.y = RandomEngine::GetRandom(-3.0f, -1.0f);
	velocity_.x = RandomEngine::GetRandom(-3.0f,3.0f);
	//velocity_ = velocity_.Normal();
	rotate_ = 0;
	velocity_ *= 4.0f;
	aliveTime_ = 40;
	isAlive_ = true;
}

//方向ベクトルから回転角を出す(下向き基準)
float DirectionToRotate(Vector2 v) {
	float theta = Dot(Vector3{ 0,1.0f,0.0f }, Vector3{ v.x,v.y,0 }.Normalize());

	theta = std::acos(theta);
	if (v.x > 0) {
		return 3.141592f - theta;
	}
	else {
		return 3.141592f + theta;
	}
}

void BlockBreakParticle::Initialize(const Vector2& postition, const Vector2& velocity, uint32_t blockType) {
	position_ = postition;
	blockType_ = blockType;
	texBase_ = { 0,0 };
	if (blockType == 2 || blockType == 3 || blockType == 4 ||  blockType == 5 || blockType == 6 || blockType == 7) {
		texBase_.x = RandomEngine::GetRandom(0.0f, 1.0f);
		if (texBase_.x > 0.5f) {
			texBase_.x = 1.0f;
		}
		else {
			texBase_.x = 0.0f;
		}
		texBase_.y = RandomEngine::GetRandom(0.0f, 1.0f);
		if (texBase_.y > 0.5f) {
			texBase_.y = 1.0f;
		}
		else {
			texBase_.y = 0.0f;
		}
		texBase_ *= 32.0f;
	}
	velocity_.y = RandomEngine::GetRandom(-2.0f, 1.0f);
	velocity_.x = RandomEngine::GetRandom(-1.0f, 1.0f);
	velocity_ += velocity*6.0f;
	rotate_ = 0;

	rotate_ = DirectionToRotate(velocity_);
	//velocity_ = velocity_.Normal();
	aliveTime_ = 40;
	if (blockType == 1){
		aliveTime_ = 30;
	}
	isAlive_ = true;
}

void BlockBreakParticle::Update() {
	velocity_ += Vector2{0.0f,0.28f};
	velocity_ *= 0.99f;
	position_ += velocity_;
	rotate_ = DirectionToRotate(velocity_);
	aliveTime_--;
	if (velocity_.x>0) {
		//rotate_ += 0.1f;
	}
	else {
		//rotate_ -= 0.1f;
	}
	if (aliveTime_<=0) {
		isAlive_ = false;
	}
}


void BlockBreakParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendParticle(position_, rotate_, texBase_, blockType_);
}
