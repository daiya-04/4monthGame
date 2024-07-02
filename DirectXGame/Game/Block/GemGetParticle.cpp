#include "GemGetParticle.h"
#include "RandomEngine/RandomEngine.h"
//int32_t GemGetParticle::response;
std::array<Vector2, 11> GemGetParticle::targetPositions_;
Player *GemGetParticle::player_ = nullptr;


void GemGetParticle::Initialize(const Vector2& postition, int32_t type) {
	targetPositions_[2] = Vector2{ 1320.0f , 96.0f + 56.0f * 1 };//brock
	targetPositions_[3] = Vector2{ 1320.0f , 96.0f + 56.0f * 1 };//block
	targetPositions_[5] = Vector2{ 1320.0f , 96.0f + 56.0f * 2 };//r
	targetPositions_[6] = Vector2{ 1320.0f , 96.0f + 56.0f * 3 };//g
	targetPositions_[7] = Vector2{ 1320.0f , 96.0f + 56.0f * 4 };//b
	position_ = postition;
	startPosition_ = position_;
	color_ = { 1.0f,1.0f,1.0f,1.0f };
	aliveTime_ = int32_t(RandomEngine::GetRandom(25.0f, 30.0f));
	aliveMax_ = aliveTime_;
	isAlive_ = true;
	
	velocity_.y = RandomEngine::GetRandom(-3.0f, -1.0f);
	velocity_.x = RandomEngine::GetRandom(-3.0f, 3.0f);
	//velocity_ = velocity_.Normal();
	velocity_ *= 4.0f;

	type_ = type;
}

void GemGetParticle::Update(const Camera& camera) {

	float t = 0;
	Vector2 pos = { 0,0 };
	switch (phase_)
	{
	case GemGetParticle::START:
		velocity_ += Vector2{ 0.0f,0.8f };
		position_ += velocity_;
		aliveTime_--;
		if (aliveTime_ <= 0) {
			startPosition_ = position_;
			aliveTime_ = kMoveMax;
			phase_ = MOVE;
		}
		break;
	case GemGetParticle::MOVE:

		t = float(aliveTime_) / float(kMoveMax);

		position_ = t * startPosition_ + (1.0f - t) * (targetPositions_[type_] + Vector2{ camera.translation_.x, camera.translation_.y });

		aliveTime_--;
		if (aliveTime_ <= 0) {
			aliveTime_ = 20;
			phase_ = END;
			switch (type_)
			{
			case 2:
			case 3:
				player_->AddRockCount(1);
				break;
			case 5://r
				player_->AddRedRock();
				break;
			case 6://g
				player_->AddGreenRock();
				break;
			case 7://b
				player_->AddBlueRock();
				break;
			default:
				break;
			}
		}
		break;
	case GemGetParticle::END:
		
		//pos.x = RandomEngine::GetRandom(-32.0f, -32.0f);
		//pos.y = RandomEngine::GetRandom(-32.0f, -32.0f);

		switch (type_)
		{
		case 5://r
			BlockTextureManager::GetInstance()->CreateStarParticleUI(pos + Vector2{ 1170.0f , 96.0f + 56.0f * 1 },-1);
			break;
		case 6://g
			BlockTextureManager::GetInstance()->CreateStarParticleUI(pos + Vector2{ 1170.0f , 96.0f + 56.0f * 2 },-2);
			break;
		case 7://b
			BlockTextureManager::GetInstance()->CreateStarParticleUI(pos + Vector2{ 1170.0f, 96.0f + 56.0f * 3},-3);
			break;
		default:
			break;
		}

		color_.w = 0;
		aliveTime_--;
		if (aliveTime_ <= 0) {
			isAlive_ = false;
		}
		break;
	default:
		break;
	}
}


void GemGetParticle::Draw() {
	BlockTextureManager::GetInstance()->AppendGemParticle(position_,type_, color_);
}
