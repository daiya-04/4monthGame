#include "EnemyManager.h"

EnemyManager* EnemyManager::GetInstance() {
	static EnemyManager instance;
	return &instance;
}

void EnemyManager::Initialize() {

	ClearEnemy();

}

void EnemyManager::AddEnemy(const Vector2& position) {

	std::shared_ptr<NormalEnemy> enemy = std::make_shared<NormalEnemy>();
	enemy->Initialize(position);
	enemy->SetBlocks(map_);
	enemy->SetPlayer(player_);
	enemies_.push_back(enemy);

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
