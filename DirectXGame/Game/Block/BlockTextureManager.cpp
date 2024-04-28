#include "BlockTextureManager.h"
#include "TextureManager.h"
#include "Block.h"
BlockTextureManager* BlockTextureManager::GetInstance() {
	static BlockTextureManager instance;
	return &instance;
}

void BlockTextureManager::LoadAllBlockTexture() {

	//マップチップに使うテクスチャを一括でロードする
	blockTextures_.clear();

	uint32_t texture = 0;
	texture = TextureManager::Load("blocks/blank.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testNormal.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testBlue.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testRed.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testIce.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testItem_01.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testItem_02.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testItem_03.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/parts.png");
	blockTextures_.push_back(texture);

	for (uint32_t index=1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		objects_[index-1]->SetTextureHandle(blockTextures_[index]);
	}
}

BlockTextureManager::BlockTextureManager() {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		std::unique_ptr<Object2dInstancing> object;
		object.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 512));
		object->SetSize({float(BaseBlock::kBlockSize_),float(BaseBlock::kBlockSize_) });
		objects_.push_back(std::move(object));
	}
}

void BlockTextureManager::ClearObject() {
	for (uint32_t index=1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		objects_[index-1]->ClearUseCount();
	}
}

void BlockTextureManager::AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize, uint32_t type) {
	BaseBlock::BlockType bType = BaseBlock::BlockType(type);
	if (bType == BaseBlock::BlockType::kNone) {
		return;
	}
	objects_[bType-1]->AppendObject(position,texBase,texSize);
}

void BlockTextureManager::DrawAll(const Camera& camera) {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		objects_[index - 1]->Draw(camera);
	}
}