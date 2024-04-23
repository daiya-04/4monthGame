#include "Block.h"

void BaseBlock::Break() {

	isBreak_ = true;
	type_ = kNone;
	//SetColor({ 1.0f,1.0f,1.0f,1.0f });

	Audio::GetInstance()->Play(digSE_);

}

void BaseBlock::Reset() {

	isBreak_ = false;
	//SetColor({ 1.0f,1.0f,1.0f,1.0f });

}

Block::Block()
{	

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

	//object_.reset(Object2d::Create(texture_, position_));
	//object_->SetSize({ float(kBlockSize_),float(kBlockSize_) });
	//object_->SetTextureArea({ 0.0f,0.0f }, { kTextureBlockSize_,kTextureBlockSize_ });

	digSE_ = Audio::GetInstance()->Load("SE/dig.wav");

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
