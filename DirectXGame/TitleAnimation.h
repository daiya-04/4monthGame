#pragma once
#include <memory>
#include <array>
#include <optional>

#include "Sprite.h"


class TitleAnimation {
private:

	enum Charactor {
		Blue,
		Orange,
	};

	enum class Mode {
		BlueIn, //BlueIn
		OrangeOut, //OrangeOut
		OrangeIn, //OrangeIn
		BlueOut, //BlurOut
	};

private:

	void BlueInInit();
	void BlueInUpdate();

	void OrangeOutInit();
	void OrangeOutUpdate();

	void OrangeInInit();
	void OrangeInUpdate();

	void BlueOutInit();
	void BlueOutUpdate();

	Mode mode_ = Mode::BlueIn;
	std::optional<Mode> modeRequest_ = std::nullopt;
	

public:

	void Init();

	void Update();

	void Draw();

	void AnimationFrameInit();

private:

	uint32_t saunaRoomTex_ = 0;
	std::unique_ptr<Sprite> saunaRoom_;

	std::array<uint32_t, 2> playerTexes_;
	std::array<std::unique_ptr<Sprite>, 2> players_;
	float playerScale_ = 1.1f;

	std::array<Vector2, 2> playerPos_;

	int32_t animationTime_ = 0;
	int32_t changeFrame_ = 5;

	int32_t animationNum_ = 0;

	const float kBlueInPos_ = -400.0f;
	const float kOrangeInPos_ = 1600.0f;
	const float kCenterPos_ = 640.0f;
	const float kPlayerYPos_ = 720.0f;
	const float kSpeed_ = 6.0f;

};

