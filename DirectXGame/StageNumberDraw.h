#pragma once
#include "Sprite.h"
#include "Vec2.h"
#include <memory>
#include <array>


class StageNumberDraw {
public:

	void Init(const Vector2& pos, const Vector2& size);

	void Draw(int32_t stageNumber);

private:

	static const int32_t kMaxDigit_ = 2;

	std::array<std::unique_ptr<Sprite>, kMaxDigit_> numberSprite_;

public:

	Vector2 position_{};
	Vector2 size_{};

};

