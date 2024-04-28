#include "Block.h"
#include "AudioManager.h"
#include "Player/Player.h"

void BaseBlock::Break(int32_t power) {

	//耐久値を減少
	durability_ -= power;

	//耐久が0以下で破壊
	if (durability_ <= 0) {

		durability_ = 0;

		//タイプに応じてプレイヤーにパラメータ強化の値を付与
		if (player_) {

			if (type_ == kSpeedBlock) {
				player_->AddSpeedParameter();
			}
			else if (type_ == kDigerBlock) {
				player_->AddDigIntervalParameter();
			}
			else if (type_ == kDigPowerBlock) {
				player_->AddDigPowerParameter();
			}
			else if (type_ == kParts) {
				player_->AddPartsCount();
			}

		}

		isBreak_ = true;
		type_ = kNone;
		SetColor({ 1.0f,1.0f,1.0f,1.0f });

	}

	digSE_->Play();

}

void BaseBlock::Reset() {

	durability_ = 3;

	isBreak_ = false;
	SetColor({ 1.0f,1.0f,1.0f,1.0f });

}

Block::Block(const Vector2& position, BlockType type)
{	

	type_ = type;
	texture_ = BlockTextureManager::GetInstance()->GetBlockTexture(type_);
	position_ = position;
	object_.reset(Object2d::Create(texture_, position_));
	digSE_ = AudioManager::GetInstance()->Load("SE/dig.wav");

}

Block::~Block()
{
}

void Block::Initialize(const Vector2& position, BlockType type) {

	type_ = type;
	texture_ = BlockTextureManager::GetInstance()->GetBlockTexture(type_);
	position_ = position;

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

	object_->SetSize({ float(kBlockSize_),float(kBlockSize_) });
	object_->SetTextureArea({ 0.0f,0.0f }, { kTextureBlockSize_,kTextureBlockSize_ });

}

void Block::Update() {

	if (type_ == kNone) {
		isBreak_ = true;
	}

	object_->SetTextureArea({ float(uvPositionX_ * kTextureBlockSize_),float(uvPositionY_ * kTextureBlockSize_) },
		{ kTextureBlockSize_,kTextureBlockSize_ });

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

}

void Block::Draw(const Camera& camera) {

	if (!isBreak_) {
		object_->Draw(camera);
	}

}
