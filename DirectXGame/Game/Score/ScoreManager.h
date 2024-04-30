#pragma once
#include <vector>

class ScoreManager
{
public:
	
	static ScoreManager* GetInstance();

	void Initialize();

	void SetNewScore(int32_t stageNum) { scores_[stageNum] = currentScore_; }

private:

	int32_t currentScore_ = 0;

	//ステージ毎のスコア記録
	std::vector<int32_t> scores_;

private:

	ScoreManager() = default;
	~ScoreManager() = default;
	ScoreManager(const ScoreManager&) = delete;
	const ScoreManager& operator=(const ScoreManager&) = delete;

};


