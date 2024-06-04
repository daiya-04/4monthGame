#include "ScoreManager.h"
#include "TextureManager.h"
#include <cmath>
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>

void Score::Init(const Vector2& pos, const Vector2& size) {

	//テクスチャ読み込み
	numTex_ = TextureManager::Load("UI/whiteNumber.png");

	pos_ = pos;
	size_ = size;
	space_ = 48.0f;

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

		numbers_[i]->SetPosition({ pos_.x + float(i - kMaxDigits_ / 2) * space_, pos_.y });

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

	//テクスチャのSS不要なので一時的にずらしている
	rankImage_->SetTextureArea({ float(value_ + 1) * 32.0f,0.0f }, { 32.0f,32.0f });

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
		if (index == 0) {
			bestScores_[index].score_.value_ = 0;
			bestScores_[index].rank_.value_ = R_N;
			goalScores_[index][R_S] = 10000;
			goalScores_[index][R_A] = goalScores_[index][R_S] / 4 * 3;
			goalScores_[index][R_B] = goalScores_[index][R_S] / 2;
			goalScores_[index][R_C] = goalScores_[index][R_S] / 4;
			goalScores_[index][R_D] = 1;
			goalScores_[index][R_N] = 0;
		}
		else {
			bestScores_[index].score_.value_ = 0;
			bestScores_[index].rank_.value_ = R_N;
			goalScores_[index][R_S] = 30000;
			goalScores_[index][R_A] = goalScores_[index][R_S] / 4 * 3;
			goalScores_[index][R_B] = goalScores_[index][R_S] / 2;
			goalScores_[index][R_C] = goalScores_[index][R_S] / 4;
			goalScores_[index][R_D] = 1;
			goalScores_[index][R_N] = 0;
		}
		
	}
	
	Load();

}

void ScoreManager::Load() {

	std::string name = "./Resources/Data/Score/bestScore.csv";

	std::ifstream file(name);

	//ファイルが無かったら新規作成
	if (!file) {

		std::ofstream newFile(name);

		if (newFile.fail()) {
			assert(false);
			return;
		}

		for (int32_t i = 0; i < kMaxStage_; i++) {

			newFile << "0,\n";

		}

		newFile.close();

	}
	//既にあるなら読み込み処理
	else {

		//一行分の文字を格納する文字列
		std::string line;

		//データを読んでスコアに格納
		for (int32_t i = 0; i < kMaxStage_; i++) {

			//一行取得
			std::getline(file, line, ',');

			bestScores_[i].score_.value_ = std::stoi(line);
			for (int32_t index = R_S; index < kRankCount; index++) {
				if (goalScores_[i][index] <= bestScores_[i].score_.value_) {
					bestScores_[i].rank_.value_ = Rate(index);
					break;
				}
			}

		}


	}

}

void ScoreManager::SaveScore() {

	std::string name = "./Resources/Data/Score/bestScore.csv";

	std::ofstream newFile(name);

	//ファイルが開けなかったら処理中止
	if (!newFile) {

		return;

	}

	for (int32_t i = 0; i < kMaxStage_; i++) {

		std::string num;

		num = std::to_string(bestScores_[i].score_.value_);

		newFile << num << ",\n";

	}

	newFile.close();

}

void ScoreManager::SetScore(int32_t stageNum, const Score& score) {

	int32_t stageIndex = stageNum - 1;

	//範囲外参照防ぐ
	stageIndex = std::clamp(stageIndex, 0, kMaxStage_ - 1);

	result_.score_.value_ = score.value_;
	for (int32_t index = R_S; index < kRankCount; index++) {
		if (goalScores_[stageIndex][index] <= result_.score_.value_) {
			result_.rank_.value_ = Rate(index);
			break;
		}
	}

	if (bestScores_[stageIndex].score_.value_ < score.value_) {
		bestScores_[stageIndex].score_.value_ = score.value_;

		for (int32_t index = R_S; index < kRankCount; index++) {
			if (goalScores_[stageIndex][index] <= bestScores_[stageIndex].score_.value_) {
				bestScores_[stageIndex].rank_.value_ = Rate(index);
				break;
			}
		}
	}

	
	

}
