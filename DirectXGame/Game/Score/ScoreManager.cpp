#include "ScoreManager.h"
#include "TextureManager.h"
#include <cmath>

void Score::Init(const Vector2& pos, const Vector2& size) {

	//テクスチャ読み込み
	numTex_ = TextureManager::Load("UI/number.png");

	pos_ = pos;
	size_ = size;

	//スプライト生成
	for (uint32_t i = 0; i < kMaxDigits_; i++) {
		numbers_[i].reset(Sprite::Create(numTex_, pos_));
		numbers_[i]->SetSize(size_);
		numbers_[i]->SetTextureArea({ 0.0f,0.0f }, size_);
	}

}

void Score::Draw() {

	int32_t digitNum = 0;

	int32_t divideNum = int32_t(std::pow(10, kMaxDigits_ - 1));

	int32_t judgeScore = value_;

	for (int32_t i = 0; i < kMaxDigits_; i++) {

		digitNum = judgeScore / divideNum;

		numbers_[i]->SetPosition({ pos_.x + float(i - kMaxDigits_ / 2) * 48.0f, pos_.y });

		numbers_[i]->SetTextureArea({ float(digitNum * 64.0f), 0.0f }, { 64.0f,64.0f });

		numbers_[i]->Draw();

		judgeScore = judgeScore % divideNum;

		divideNum = int32_t(divideNum * 0.1f);
	}

}

void Score::SetScore(const Score& score) {
	value_ = score.value_;
}

void Rank::Init(const Vector2& pos, const Vector2& size) {

	rankTex_ = TextureManager::Load("rank.png");

	pos_ = pos;
	size_ = size;

	rankImage_.reset(Sprite::Create(rankTex_, pos_));
	rankImage_->SetSize(size_);
	rankImage_->SetTextureArea({}, { 32.0f,32.0f });

}

void Rank::Draw() {

	rankImage_->SetPosition(pos_);
	rankImage_->SetSize(size_);
	rankImage_->SetTextureArea({ float(value_) * 32.0f,0.0f }, { 32.0f,32.0f });

	rankImage_->Draw();

}

void Rank::SetRank(const Rank& rank) {
	value_ = rank.value_;
}

ScoreManager::ScoreManager() {
	Initialize();
}

ScoreManager* ScoreManager::GetInstance() {
	static ScoreManager instance;
	return &instance;

}

void ScoreManager::Initialize() {

	

	//目標スコアの設定
	for (int32_t index = 0; index < kMaxStage_; index++) {
		bestScores_[index].score_.value_ = 0;
		bestScores_[index].rank_.value_ = R_SS;
		goalScores_[index][R_SS] = 10000;
		goalScores_[index][R_S] = 8000;
		goalScores_[index][R_A] = 6000;
		goalScores_[index][R_B] = 4000;
		goalScores_[index][R_C] = 2000;
		goalScores_[index][R_D] = 1;
		goalScores_[index][R_N] = 0;
	}
	////stage1
	//goalScores_[R_SS] = 10000;
	//goalScores_[R_S] = 8000;
	//goalScores_[R_A] = 6000;
	//goalScores_[R_B] = 4000;
	//goalScores_[R_C] = 2000;
	//goalScores_[R_D] = 1;
	//goalScores_[R_N] = 0;
	////stage2
	//goalScores_[R_SS] = 10000;
	//goalScores_[R_S] = 8000;
	//goalScores_[R_A] = 6000;
	//goalScores_[R_B] = 4000;
	//goalScores_[R_C] = 2000;
	//goalScores_[R_D] = 1;
	//goalScores_[R_N] = 0;

}

//Rank ScoreManager::JudgeRank(int32_t stageNum) {
//
//	for (int32_t i = 0; i < kRankCount; i++) {
//
//		if (scores_[stageNum] >= baseScores_[stageNum][i]) {
//
//			return Rank(i);
//
//		}
//
//	}
//
//	return R_N;
//
//}

void ScoreManager::SetScore(int32_t stageNum, const Score& score) {

	int32_t stageIndex = stageNum - 1;

	if (bestScores_[stageIndex].score_.value_ < score.value_) {
		bestScores_[stageIndex].score_.value_ = score.value_;
	}

	result_.score_.value_ = score.value_;

	for (int32_t index = R_SS; index < kRankCount; index++) {
		if (goalScores_[stageIndex][index] < score.value_) {
			if (bestScores_[stageIndex].rank_.value_ < Rate(index)) {
				bestScores_[stageIndex].rank_.value_ = Rate(index);
				break;
			}
			result_.rank_.value_ = Rate(index);
			break;
		}
	}
	

}
