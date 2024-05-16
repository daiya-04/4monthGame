#include "UpgradeSystem.h"
#include "TextureManager.h"
#include "Player/Player.h"

UpgradeSystem::UpgradeSystem()
{

	furnaceTexture_ = TextureManager::GetInstance()->Load("stageObject/furnace.png");

	furnace_.reset(Object2d::Create(furnaceTexture_, {Block::kBlockSize_ * 19.5f, Block::kBlockSize_ * 2.5f}));
	furnace_->SetSize({ Block::kBlockSize_ * 2.0f,Block::kBlockSize_ * 4.0f });

}

UpgradeSystem::~UpgradeSystem()
{
}

void UpgradeSystem::Initialize() {



}

void UpgradeSystem::Update() {



}

void UpgradeSystem::Draw(const Camera& camera) {

	furnace_->Draw(camera);

}
