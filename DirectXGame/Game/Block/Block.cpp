#include "Block.h"
#include "AudioManager.h"
#include "Player/Player.h"
#include "BlockBreakParticle.h"
#include "RandomEngine/RandomEngine.h"
#include "Stage/Magma.h"

Score* BaseBlock::score_;

void BaseBlock::Break(int32_t power) {

	//耐久値を減少
	durability_ -= power;

	//耐久が0以下で破壊
	if (durability_ <= 0) {

		durability_ = 0;

		score_->AddScore(100);

		//タイプに応じてプレイヤーにパラメータ強化の値を付与
		if (player_) {

			if (type_ == kMagma || type_ == kSnow) {
				player_->AddRockCount(defaultDurability_ / 3 + 1);
			}
			else if (type_ == kBlueBlock) {
				player_->AddBlueRock();
			}
			else if (type_ == kGreenBlock) {
				player_->AddGreenRock();
			}
			else if (type_ == kRedBlock) {
				player_->AddRedRock();
			}
			else if (type_ == kDownMagma) {
				//ラインを1000下げ、300フレーム止める
				magma_->baseMagmaLine_ += 1000.0f;
				magma_->freezeTime_ = 300;
				magma_->easingT_ = 0.0f;
			}
			else if (type_ == kGoldBlock) {
				player_->SetIsClear(true);
			}

		}
		int createNum = int(RandomEngine::GetRandom(8.0f,12.0f));
		for (int i = 0; i < createNum;i++) {
			BlockTextureManager::GetInstance()->CreateParticle(position_, type_);
		}
	isBreak_ = true;
	type_ = kNone;
	//SetColor({ 1.0f,1.0f,1.0f,1.0f });
	
	}
	else {
		score_->AddScore(10);
		int createNum = int(RandomEngine::GetRandom(2.0f, 5.0f));
		for (int i = 0; i < createNum; i++) {
			Vector2 velocity = player_->GetPosition() - position_;
			Vector2 pos = position_ + velocity*0.5f;
			velocity = velocity.Normalize();
			BlockTextureManager::GetInstance()->CreateParticle(pos,velocity, type_);
		}
	}

	digSE_->Play();

}

void BaseBlock::Reset() {

	durability_ = 3;

	isBreak_ = false;
	//SetColor({ 1.0f,1.0f,1.0f,1.0f });

}

Block::Block(const Vector2& position, BlockType type)
{	

	defaultType_ = type;
	type_ = type;
	position_ = position;
	/*object_.reset(Object2d::Create(texture_, position_));*/
	digSE_ = AudioManager::GetInstance()->Load("SE/dig.wav");

}

Block::~Block()
{
}

void Block::Initialize(const Vector2& position, BlockType type) {

	defaultType_ = type;
	type_ = type;
	position_ = position;

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

	//object_.reset(Object2d::Create(texture_, position_));
	//object_->SetSize({ float(kBlockSize_),float(kBlockSize_) });
	//object_->SetTextureArea({ 0.0f,0.0f }, { kTextureBlockSize_,kTextureBlockSize_ });

}

void Block::Update() {

	if (type_ == kNone) {
		isBreak_ = true;
	}

	//object_->SetTextureArea({ float(uvPositionX_ * kTextureBlockSize_),float(uvPositionY_ * kTextureBlockSize_) },
		//{ kTextureBlockSize_,kTextureBlockSize_ });

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

	if (type_ == kGoldBlock) {
		BlockTextureManager::GetInstance()->CreateStarParticle(position_,0);
	}

	if (!isBreak_ && type_ != kNone) {
		//当たり判定
		AABB2D collision = collision_;
		collision.min.x -= 1.0f;
		collision.min.y -= 1.0f;
		collision.max.x += 1.0f;
		collision.max.y += 1.0f;
		if (player_->GetPrePosition() != player_->GetPosition() && IsCollision(collision, player_->GetCollision())) {
			Vector2 velocity = player_->GetPosition() - position_;
			Vector2 pos = position_ + velocity * 0.7f;
			velocity = velocity.Normalize();
			BlockTextureManager::GetInstance()->CreateSandParticle(pos, type_);
		}
	}
}

void Block::Draw(const Camera& camera) {

	if (!isBreak_) {
		//object_->Draw(camera);
		//切り替わるタイプのブロックだったら両方入れる
		if (type_ == BlockType::kSnow || type_ == BlockType::kMagma) {
			BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, kMagma);
			BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, kSnow);
		}
		else {
			BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, type_);
		}
	}

}
