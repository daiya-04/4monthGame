#pragma once
#include "Sprite.h"
#include "Vec2.h"
#include <memory>
#include <array>


class StageNumberDraw {
public:

	void Init(const Vector2& pos, const Vector2& size);

	void Draw();

	void SetStageNumber(int32_t stageNumber) { stageNumber_ = stageNumber; }

	void AnimationInit();

	void AnimationUpdate();

private:

	static const int32_t kMaxDigit_ = 2;

	std::array<std::unique_ptr<Sprite>, kMaxDigit_> numberSprite_;

	int32_t stageNumber_ = 1;

	struct WorkBounding {
		float param_ = 0.0f;
		//振幅
		float amplitude_ = 20.0f;
		//サイクル<frame>
		int32_t cycle_ = 60;

		Vector2 addPos_{};
	};

	WorkBounding bounding_;

public:

	Vector2 position_{};
	Vector2 size_{};

};

