#include "Block.h"
#include "BlockTextureManager.h"

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
	collision_.max = { position_.x + kBlockHalfSize_, position_.y + kBlockHalfSize_ };

	object_.reset(Object2d::Create(texture_, position_));
	object_->SetSize({ float(kBlockSize_),float(kBlockSize_) });
	object_->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

}

void Block::Update() {

	object_->SetTextureArea({ float(uvPositionX_ * 64.0f),float(uvPositionY_ * 64.0f) }, { 64.0f,64.0f });

}

void Block::Draw(const Camera& camera) {

	object_->Draw(camera);

}
