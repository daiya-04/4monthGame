#include "Block.h"
#include "AudioManager.h"
#include "Player/Player.h"
#include "BlockBreakParticle.h"
#include "RandomEngine/RandomEngine.h"
void BaseBlock::Break(int32_t power) {

	//耐久値を減少
	durability_ -= power;

	//耐久が0以下で破壊
	if (durability_ <= 0) {

		durability_ = 0;

		scoreManager_->AddScore(100);

		//タイプに応じてプレイヤーにパラメータ強化の値を付与
		if (player_) {

			if (type_ == kMagma || type_ == kSnow) {
				player_->AddRockCount(defaultDurability_ / 3 + 1);
			}
			else if (type_ == kSpeedBlock) {
				player_->AddBlueRock();
			}
			else if (type_ == kDigerBlock) {
				player_->AddGreenRock();
			}
			else if (type_ == kDigPowerBlock) {
				player_->AddRedRock();
			}

		}
		int createNum = int(RandomEngine::GetRandom(3.0f,5.0f));
		for (int i = 0; i < createNum;i++) {
			BlockTextureManager::GetInstance()->CreateParticle(position_, type_);
		}
	isBreak_ = true;
	type_ = kNone;
	//SetColor({ 1.0f,1.0f,1.0f,1.0f });
	
	}
	else {
		scoreManager_->AddScore(10);
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

	type_ = type;
	texture_ = BlockTextureManager::GetInstance()->GetBlockTexture(type_);
	position_ = position;
	/*object_.reset(Object2d::Create(texture_, position_));*/
	digSE_ = AudioManager::GetInstance()->Load("SE/dig.wav");

}

Block::~Block()
{
}

void Block::Initialize(const Vector2& position, BlockType type) {

	scoreManager_ = ScoreManager::GetInstance();

	type_ = type;
	texture_ = BlockTextureManager::GetInstance()->GetBlockTexture(type_);
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

}

void Block::Draw(const Camera& camera) {

	if (!isBreak_) {
		//object_->Draw(camera);
		BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ },type_);
	}

}
