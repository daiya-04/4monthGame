#include "EnemyManager.h"
#include "DirectXCommon.h"
EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize() {
	CreateInstances();
	ClearEnemy();

}

void EnemyManager::AddEnemy(const Vector2& position, const BaseEnemy::Type& type) {

	switch (type)
	{
	default:
	case BaseEnemy::Type::kNormal:

		{
			std::shared_ptr<NormalEnemy> enemy = std::make_shared<NormalEnemy>();
			enemy->Initialize(position);
			enemy->SetBlocks(map_);
			enemy->SetPlayer(player_);
			enemies_.push_back(enemy);
		}

		break;
	case BaseEnemy::Type::kNeedle:

		{
			std::shared_ptr<NeedleEnemy> enemy = std::make_shared<NeedleEnemy>();
			enemy->Initialize(position);
			enemy->SetBlocks(map_);
			enemy->SetPlayer(player_);
			enemies_.push_back(enemy);
		}

		break;
	}

}

void EnemyManager::Update() {

	enemies_.remove_if([](auto& enemy) {

		if (enemy->GetIsDead()) {
			return true;
		}

		return false;

		});

	for (auto& enemy : enemies_) {

		enemy->Update();

	}

}

void EnemyManager::Draw(const Camera& camera) {

	for (std::unique_ptr<Object2dInstancing>& data : enemyObjects_) {
		data->ClearUseCount();
	}

	for (auto& enemy : enemies_) {
		//enemy->Draw(camera);
		enemy->SetDrawData(enemyObjects_[uint32_t(enemy->GetType())].get());
	}
	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (std::unique_ptr<Object2dInstancing>& data : enemyObjects_) {
		data->Draw(camera);
	}
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
}

void EnemyManager::CreateInstances() {
	enemyObjects_[uint32_t(BaseEnemy::Type::kNormal)].reset(Object2dInstancing::Create(TextureManager::GetInstance()->Load("enemy/enemy.png"), Vector2{ 0,0 }, 128));
	enemyObjects_[uint32_t(BaseEnemy::Type::kNeedle)].reset(Object2dInstancing::Create(TextureManager::GetInstance()->Load("blocks/needle.png"), Vector2{ 0,0 }, 128));

}