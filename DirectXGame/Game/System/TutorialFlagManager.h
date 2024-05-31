#pragma once
#include <array>
#include <vector>
#include "Player/Player.h"
#include "Stage/Magma.h"
#include "Scroll/Scroll.h"

class TutorialFlagManager
{
public:
	
	static TutorialFlagManager* GetInstance();

	static const int32_t kMaxTutorial_ = 9;

	void Initialize();

	void Update();

	void UpdateInText();

	void SetPlayer(Player* player) { player_ = player; }

	void SetMagma(Magma* magma) { magma_ = magma; }

	void SetScroll(Scroll* scroll) { scroll_ = scroll; }

	bool GetFlags(int32_t num) {
		num = std::clamp(num, 0, kMaxTutorial_ - 1);
		return flags_[num];
	}

private:

	Player* player_ = nullptr;

	Magma* magma_ = nullptr;

	Scroll* scroll_ = nullptr;

	Vector2 tutorialCameraPosition1_{};
	Vector2 tutorialCameraPosition2_{};

	int32_t tutorialNumber_ = 0;

	bool isEndTutorial_ = false;

	std::array<bool, kMaxTutorial_> flags_;

	TutorialFlagManager() = default;
	~TutorialFlagManager() = default;
	TutorialFlagManager(const TutorialFlagManager&) = delete;
	const TutorialFlagManager& operator=(const TutorialFlagManager&) = delete;

};
