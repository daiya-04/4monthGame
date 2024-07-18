#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize() {

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

	for (auto& enemy : enemies_) {
		enemy->Draw(camera);
	}

}
