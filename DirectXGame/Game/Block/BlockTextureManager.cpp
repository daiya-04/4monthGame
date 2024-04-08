#include "BlockTextureManager.h"
#include "TextureManager.h"

BlockTextureManager* BlockTextureManager::GetInstance() {
	static BlockTextureManager instance;
	return &instance;
}

void BlockTextureManager::LoadAllBlockTexture() {

	//マップチップに使うテクスチャを一括でロードする
	blockTextures_.clear();

	int32_t texture = 0;
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

}
