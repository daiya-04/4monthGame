#include "ScoreManager.h"
#include "TextureManager.h"
#include <cmath>

ScoreManager::ScoreManager() {

	Initialize();

}

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
	baseScores_[0][R_D] = 1;
	baseScores_[0][R_N] = 0;
	//stage2
	baseScores_[1][R_SS] = 10000;
	baseScores_[1][R_S] = 8000;
	baseScores_[1][R_A] = 6000;
	baseScores_[1][R_B] = 4000;
	baseScores_[1][R_C] = 2000;
	baseScores_[1][R_D] = 1;
	baseScores_[1][R_N] = 0;

	//テクスチャ読み込み
	numTex_ = TextureManager::GetInstance()->Load("UI/number.png");

	//スプライト生成
	for (uint32_t i = 0; i < kMaxDigits_; i++) {

		numbers_[i].reset(Sprite::Create(numTex_, { 0.0f,0.0f }));
		numbers_[i]->SetSize({ 64.0f,64.0f });
		numbers_[i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

	}

}

void ScoreManager::Load() {



}

void ScoreManager::SaveScore() {

}

Rank ScoreManager::JudgeRank(int32_t stageNum) {

	for (int32_t i = 0; i < kRankCount; i++) {

		if (scores_[stageNum] >= baseScores_[stageNum][i]) {

			return Rank(i);

		}

	}

	return R_N;

}

void ScoreManager::DrawCurrentScore(const Vector2& position) {

	int32_t digitNum = 0;

	int32_t divideNum = int32_t(std::pow(10, kMaxDigits_ - 1));

	int32_t judgeScore = currentScore_;

	for (int32_t i = 0; i < kMaxDigits_; i++) {

		digitNum = judgeScore / divideNum;

		numbers_[i]->SetPosition({ position.x + float(i - kMaxDigits_ / 2) * 48.0f, position.y});

		numbers_[i]->SetTextureArea({ float(digitNum * 64.0f), 0.0f }, { 64.0f,64.0f });

		numbers_[i]->Draw();

		judgeScore = judgeScore % divideNum;

		divideNum = int32_t(divideNum * 0.1f);

	}

}

void ScoreManager::DrawBestScore(int32_t stageNum, const Vector2& position) {

	if (stageNum >= kMaxStage_) {
		return;
	}

	int32_t digitNum = 0;

	int32_t divideNum = int32_t(std::pow(10, kMaxDigits_ - 1));

	int32_t judgeScore = scores_[stageNum];

	for (int32_t i = 0; i < kMaxDigits_; i++) {

		digitNum = judgeScore / divideNum;

		numbers_[i]->SetPosition({ position.x - float(i - kMaxDigits_ / 2) * 48.0f, position.y });

		numbers_[i]->SetTextureArea({ float(digitNum * 64.0f), 0.0f }, { 64.0f,64.0f });

		numbers_[i]->Draw();

		judgeScore = judgeScore % divideNum;

	}

}
