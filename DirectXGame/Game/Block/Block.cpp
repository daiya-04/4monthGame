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

	sprite_.reset(Sprite::Create(texture_, position_));

}

void Block::Update() {



}

void Block::Draw() {

	sprite_->Draw();

}
