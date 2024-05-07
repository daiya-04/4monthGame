#include "ScoreManager.h"

ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;
	return &instance;

}

void ScoreManager::Initialize() {

	//目標スコアの設定

	//stage1
	baseScores_[0][R_SS] = 10000;
	baseScores_[0][R_S] = 8000;
	baseScores_[0][R_A] = 6000;
	baseScores_[0][R_B] = 4000;
	baseScores_[0][R_C] = 2000;
	baseScores_[0][R_D] = 0;
	//stage2
	baseScores_[1][R_SS] = 10000;
	baseScores_[1][R_S] = 8000;
	baseScores_[1][R_A] = 6000;
	baseScores_[1][R_B] = 4000;
	baseScores_[1][R_C] = 2000;
	baseScores_[1][R_D] = 0;

}

void ScoreManager::Load() {



}

void ScoreManager::SaveScore() {

}
