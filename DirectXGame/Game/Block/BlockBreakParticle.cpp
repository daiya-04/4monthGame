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
	velocity_.y = RandomEngine::GetRandom(-8.0f,-4.0f);
	velocity_.x = RandomEngine::GetRandom(-2.0f, 2.0f);
	velocity_ += velocity*7.0f;
	rotate_ = 0;
	//velocity_ = velocity_.Normal();
	aliveTime_ = 40;
	isAlive_ = true;
}

void BlockBreakParticle::Update() {
	velocity_ += Vector2{0.0f,0.8f};
	position_ += velocity_;
	aliveTime_--;
	if (velocity_.x>0) {
		rotate_ += 0.1f;
	}
	else {
		rotate_ -= 0.1f;
	}
	if (aliveTime_<=0) {
		isAlive_ = false;
	}
}


void BlockBreakParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendParticle(position_, rotate_, texBase_, blockType_);
}
