#pragma once
#include "Enemy.h"
#include <memory>
#include <vector>
#include <list>
#include "Camera.h"
#include "Stage/StageConstant.h"
#include "Player/Player.h"

class EnemyManager
{
public:
	
	static EnemyManager* GetInstance();

	void Initialize();

	void SetMap(std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* map) { map_ = map; }

	void SetPlayer(Player* player) { player_ = player; }

	void AddEnemy(const Vector2& position, const BaseEnemy::Type& type = BaseEnemy::Type::kNormal);

	void Update();

	void Draw(const Camera& camera);

	void ClearEnemy() { enemies_.clear(); }

	std::list<std::shared_ptr<BaseEnemy>>& GetEnemies() { return enemies_; }

private:

	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* map_ = nullptr;

	Player* player_ = nullptr;

	std::list<std::shared_ptr<BaseEnemy>> enemies_;

private:

	EnemyManager() = default;
	~EnemyManager() = default;
	EnemyManager(const EnemyManager&) = delete;
	const EnemyManager& operator=(const EnemyManager&) = delete;

	//uint32_t textureHandles[uint32_t(BaseEnemy::Type::kCount)];
	std::array<std::unique_ptr<Object2dInstancing>, uint32_t(BaseEnemy::Type::kCount)> enemyObjects_;
	void CreateInstances();
};
