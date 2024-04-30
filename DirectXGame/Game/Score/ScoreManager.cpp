#include "ScoreManager.h"
#include "DefaultScene.h"

ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;
	return &instance;

}

void ScoreManager::Initialize() {

	scores_.resize(DefaultScene::kMaxStage_);

}
