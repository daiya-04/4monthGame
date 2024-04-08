#include "Block.h"

void BaseBlock::Break() {

	isBreak_ = true;
	type_ = kNone;

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

	object_.reset(Object2d::Create(texture_, position_));
	object_->SetSize({ float(kBlockSize_),float(kBlockSize_) });
	object_->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

}

void Block::Update() {

	if (type_ == kNone) {
		isBreak_ = true;
	}

	object_->SetTextureArea({ float(uvPositionX_ * 64.0f),float(uvPositionY_ * 64.0f) }, { 64.0f,64.0f });

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

}

void Block::Draw(const Camera& camera) {

	if (!isBreak_) {
		object_->Draw(camera);
	}

}
