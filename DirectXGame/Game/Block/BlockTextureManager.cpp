#include "BlockTextureManager.h"
#include "TextureManager.h"
#include "Block.h"
#include "BlockBreakParticle.h"
BlockTextureManager* BlockTextureManager::GetInstance() {
	static BlockTextureManager instance;
	return &instance;
}

void BlockTextureManager::LoadAllBlockTexture() {

	//マップチップに使うテクスチャを一括でロードする
	blockTextures_.clear();

	uint32_t texture = 0;
	texture = TextureManager::Load("blocks/defaultRocks.png");
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
		objects_[index-1]->SetTextureHandle(blockTextures_[0]);
		breakParticles_[index - 1]->SetTextureHandle(blockTextures_[0]);
	}

	//壊せないブロック
	objects_[Block::kUnbreakable - 1]->SetColor({ 0.4f,0.4f,0.4f,1.0f });
	//極寒
	objects_[Block::kSnow - 1]->SetColor({ 0.6f,0.3f,0.1f,1.0f });
	//灼熱
	objects_[Block::kMagma - 1]->SetColor({ 0.6f,0.2f,0.2f,1.0f });
	//氷
	objects_[Block::kIceBlock - 1]->SetColor({ 0.7f,0.7f,1.0f,1.0f });
	//速度
	objects_[Block::kSpeedBlock - 1]->SetColor({ 0.1f,0.1f,0.3f,1.0f });
	//採掘速度
	objects_[Block::kDigerBlock - 1]->SetColor({ 0.1f,0.3f,0.1f,1.0f });
	//パワーーー！！！
	objects_[Block::kDigPowerBlock - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });
	//パワーーー！！！
	objects_[Block::kDownMagma - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });

	//壊せないブロック
	breakParticles_[Block::kUnbreakable - 1]->SetColor({ 0.4f,0.4f,0.4f,1.0f });
	//極寒
	breakParticles_[Block::kSnow - 1]->SetColor({ 0.6f,0.3f,0.1f,1.0f });
	//灼熱
	breakParticles_[Block::kMagma - 1]->SetColor({ 0.6f,0.2f,0.2f,1.0f });
	//氷
	breakParticles_[Block::kIceBlock - 1]->SetColor({ 0.7f,0.7f,1.0f,1.0f });
	//速度
	breakParticles_[Block::kSpeedBlock - 1]->SetColor({ 0.1f,0.1f,0.3f,1.0f });
	//採掘速度
	breakParticles_[Block::kDigerBlock - 1]->SetColor({ 0.1f,0.3f,0.1f,1.0f });
	//パワーーー！！！
	breakParticles_[Block::kDigPowerBlock - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });
	//パワーーー！！！
	breakParticles_[Block::kDownMagma - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });

}

BlockTextureManager::BlockTextureManager() {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		std::unique_ptr<Object2dInstancing> object;
		object.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 512));
		object->SetSize({float(BaseBlock::kBlockSize_),float(BaseBlock::kBlockSize_) });
		objects_.push_back(std::move(object));
		object.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 32));
		object->SetSize({ float(BaseBlock::kBlockSize_/2),float(BaseBlock::kBlockSize_/2) });
		breakParticles_.push_back(std::move(object));
	}
}

void BlockTextureManager::ClearObject() {
	for (uint32_t index=1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		objects_[index-1]->ClearUseCount();
		breakParticles_[index - 1]->ClearUseCount();
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

void BlockTextureManager::AppendParticle(const Vector2& position, uint32_t type) {
	BaseBlock::BlockType bType = BaseBlock::BlockType(type);
	if (bType == BaseBlock::BlockType::kNone) {
		return;
	}
	breakParticles_[bType - 1]->AppendObject(position, Vector2{0,0}, Vector2{ 32.0f,32.0f });
}

void BlockTextureManager::DrawParticle(const Camera& camera) {
	for (std::unique_ptr<BlockBreakParticle>& data : breakParticleDatas_) {
		data->Draw();
	}
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		breakParticles_[index - 1]->Draw(camera);
	}
}

void BlockTextureManager::CreateParticle(const Vector2& position, uint32_t type) {
	std::unique_ptr<BlockBreakParticle> particle;
	particle.reset(new BlockBreakParticle);
	particle->Initialize(position,type);
	breakParticleDatas_.push_back(std::move(particle));
}

void BlockTextureManager::UpdateParticle() {
	breakParticleDatas_.remove_if([](std::unique_ptr<BlockBreakParticle>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<BlockBreakParticle> &data : breakParticleDatas_) {
		data->Update();
	}
}