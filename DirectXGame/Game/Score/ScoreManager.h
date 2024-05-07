#pragma once
#include <vector>
#include <array>
#include "DefaultScene.h"

class ScoreManager
{
public:
	
	//表示ランク(仮)。SSの目標スコアを10000で設定したとして、10000取れたらSS表示、9999ならS表示する仕様
	enum Rank {
		R_SS, //SSランク, 基準高めで設定、やり込む人向けの裏ランク的な
		R_S, //Sランク, これ取れたら上手いって言えるくらいの設定
		R_A, //Aランク, ちょっとミスしてもとれる難易度で
		R_B, //Bランク, 初見プレイでこのくらいになるような設定
		R_C, //Cランク, かなりミスしてもこのランクは取れるように
		R_D, //Dランク, 最低ランクなので目標スコアは基本0でいい
		
		kRankCount, //ランクの最大数, 配列とかの変数でのみ使用

	};

	static ScoreManager* GetInstance();

	void Initialize();

	void SetNewScore(int32_t stageNum) { scores_[stageNum] = currentScore_; }

	int32_t GetScore(int32_t stageNum) { return scores_[stageNum]; }

	int32_t GetBaseScore(int32_t stageNum, Rank rank) { return baseScores_[stageNum][rank]; }

private:

	int32_t currentScore_ = 0;

	//ステージ毎のスコア記録
	std::array<int32_t, DefaultScene::kMaxStage_> scores_;

	//ステージ毎に決められた目標スコア
	std::array<std::array<int32_t, kRankCount>, DefaultScene::kMaxStage_> baseScores_;

	//スコアデータの読み込み
	void Load();

	//スコアの書き込み
	void SaveScore();

private:

	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	const ScoreManager& operator=(const ScoreManager&) = delete;

};


