#pragma once
#include <vector>
#include <array>
#include "Sprite.h"
#include <memory>
#include <map>

//表示ランク(仮)。SSの目標スコアを10000で設定したとして、10000取れたらSS表示、9999ならS表示する仕様
enum Rate : int32_t {
	R_SS, //SSランク, 基準高めで設定、やり込む人向けの裏ランク的な
	R_S, //Sランク, これ取れたら上手いって言えるくらいの設定
	R_A, //Aランク, ちょっとミスしてもとれる難易度で
	R_B, //Bランク, 初見プレイでこのくらいになるような設定
	R_C, //Cランク, かなりミスしてもこのランクは取れるように
	R_D, //Dランク, 最低ランクなので目標スコアは基本1でいい
	R_N, //未プレイ時用のランク判定。基本0

	kRankCount, //ランクの最大数, 配列とかの変数でのみ使用
};

class Score {
private:
	friend class ScoreManager;

public:
	//最大桁数
	static const int32_t kMaxDigits_ = 8;

public:

	void Init(const Vector2& pos, const Vector2& size);

	void AddScore(int32_t value) {
		value_ += value;
		value_ = std::clamp(value_, 0, 99999999);
	}

	int32_t GetScore() const { return value_; }

	void SetScore(const Score& score);

	void Draw();

private:
	int32_t value_ = 0;

	//スコアの数字
	uint32_t numTex_ = 0;
	std::array<std::unique_ptr<Sprite>, kMaxDigits_> numbers_;

public:

	Vector2 pos_{};
	Vector2 size_{};

};

class Rank {
private:
	friend class ScoreManager;
public:

	void Init(const Vector2& pos, const Vector2& size);

	void SetRank(const Rank& rank);

	void Draw();

private:

	Rate value_ = R_N;

	//スコアのランク
	uint32_t rankTex_ = 0;
	std::unique_ptr<Sprite> rankImage_;

public:

	Vector2 pos_{};
	Vector2 size_{};

};

class ScoreManager {
public:

	struct Result {
		Rank rank_;
		Score score_;
	};

	static const int32_t kMaxStage_ = 10;

	static ScoreManager* GetInstance();

	void Initialize();

	//基本的にゲームシーンからクリアシーンに切り替える直前に使う
	void SetScore(int32_t stageNum, const Score& score);

	Score& GetBestScore(int32_t stageIndex) { return bestScores_[stageIndex].score_; }
	Rank& GetBestRank(int32_t stageIndex) { return bestScores_[stageIndex].rank_; }

	//クリア時にこれから値取得して
	Result& GetResult() { return result_; }

	void Load();

	void SaveScore();

private:

	//ステージ毎のスコア記録
	//std::array<int32_t, kMaxStage_> bestScores_;
	std::array<Result, kMaxStage_> bestScores_;

	//インゲーム中のスコア、クリア時の表示にもこれ使う
	Result result_;

	//ステージ毎に決められた目標スコア
	std::array<std::array<int32_t, kRankCount>, kMaxStage_> goalScores_;

private:

	ScoreManager();
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	const ScoreManager& operator=(const ScoreManager&) = delete;

};


