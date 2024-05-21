#pragma once
#include <vector>
#include <array>
#include "Sprite.h"
#include <memory>

//表示ランク(仮)。SSの目標スコアを10000で設定したとして、10000取れたらSS表示、9999ならS表示する仕様
enum Rank : int32_t {
	R_SS, //SSランク, 基準高めで設定、やり込む人向けの裏ランク的な
	R_S, //Sランク, これ取れたら上手いって言えるくらいの設定
	R_A, //Aランク, ちょっとミスしてもとれる難易度で
	R_B, //Bランク, 初見プレイでこのくらいになるような設定
	R_C, //Cランク, かなりミスしてもこのランクは取れるように
	R_D, //Dランク, 最低ランクなので目標スコアは基本1でいい
	R_N, //未プレイ時用のランク判定。基本0

	kRankCount, //ランクの最大数, 配列とかの変数でのみ使用

};

class ScoreManager
{
public:

	static const int32_t kMaxStage_ = 10;

	//最大桁数
	static const int32_t kMaxDigits_ = 8;

	static ScoreManager* GetInstance();

	void Initialize();

	void SetNewScore(int32_t stageNum) { scores_[stageNum] = currentScore_; }

	int32_t GetScore(int32_t stageNum) { return scores_[stageNum]; }

	int32_t GetBaseScore(int32_t stageNum, Rank rank) { return baseScores_[stageNum][rank]; }

	int32_t GetCurrentScore() const { return currentScore_; }

	void AddScore(int32_t value) {

		currentScore_ += value;

		//スコアは0で止める
		if (currentScore_ < 0) {
			currentScore_ = 0;
		}

	}

	void ResetScore() { currentScore_ = 0; }

	//ステージのスコアに応じてランク判定
	Rank JudgeRank(int32_t stageNum);

	//ゲーム中のスコア表示
	void DrawCurrentScore(const Vector2& position);

	//ステージ毎のベストスコア表示
	void DrawBestScore(int32_t stageNum, const Vector2& position);

	//スコアの書き込み
	void SaveScore();

private:

	int32_t currentScore_ = 0;

	//ステージ毎のスコア記録
	std::array<int32_t, kMaxStage_> scores_;

	//ステージ毎に決められた目標スコア
	std::array<std::array<int32_t, kRankCount>, kMaxStage_> baseScores_;

	//スコアの数字
	std::array<std::unique_ptr<Sprite>, kMaxDigits_> numbers_;

	//スコアデータの読み込み
	void Load();

	uint32_t numTex_;

private:

	ScoreManager();
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	const ScoreManager& operator=(const ScoreManager&) = delete;

};


