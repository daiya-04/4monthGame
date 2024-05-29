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
				crystalSE_->Play();
			}
			else if (type_ == kGreenBlock) {
				player_->AddGreenRock();
				crystalSE_->Play();
			}
			else if (type_ == kRedBlock) {
				player_->AddRedRock();
				crystalSE_->Play();
			}
			else if (type_ == kDownMagma) {
				//ラインを1000下げ、300フレーム止める
				magma_->baseMagmaLine_ += 1000.0f;
				magma_->freezeTime_ = 300;
				magma_->easingT_ = 0.0f;
			}
			else if (type_ == kGoldBlock) {
				player_->SetIsClear(true);
				crystalSE_->Play();
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
	}

	//耐久力に応じてSE変更
	if (durability_ >= defaultDurability_ - defaultDurability_ / 2) {
		digLowSE_->Play();
	}
	else if (durability_ > 0) {
		digMidSE_->Play();
	}
	else if (durability_ <= 0) {
		digHighSE_->Play();
	}

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
	digLowSE_ = AudioManager::GetInstance()->Load("SE/dig_low.mp3");
	digMidSE_ = AudioManager::GetInstance()->Load("SE/dig_mid.mp3");
	digHighSE_ = AudioManager::GetInstance()->Load("SE/dig_high.mp3");
	crystalSE_ = AudioManager::GetInstance()->Load("SE/crystal_get.mp3");

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
