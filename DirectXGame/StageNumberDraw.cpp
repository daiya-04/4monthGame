#include "StageNumberDraw.h"
#include "TextureManager.h"

#include <numbers>

void StageNumberDraw::Init(const Vector2& pos, const Vector2& size) {

	uint32_t numberTex = TextureManager::Load("UI/number.png");

	position_ = pos;
	size_ = size;

	for (size_t index = 0; index < kMaxDigit_; index++) {
		numberSprite_[index].reset(Sprite::Create(numberTex, {}));
		numberSprite_[index]->SetSize(size_);
	}

}

void StageNumberDraw::Draw() {

	if (stageNumber_ < 10) {
		numberSprite_[0]->SetPosition(position_ + bounding_.addPos_);
		numberSprite_[0]->SetTextureArea({ 64.0f * stageNumber_,0.0f }, { 64.0f,64.0f });
		numberSprite_[0]->Draw();
	}
	else {

		int32_t digitNum = 0;

		int32_t divideNum = int32_t(std::pow(10, kMaxDigit_ - 1));

		int32_t judgeNumber = stageNumber_;

		numberSprite_[0]->SetPosition({ position_.x - 25.0f, position_.y + bounding_.addPos_.y});
		numberSprite_[1]->SetPosition({ position_.x + 25.0f, position_.y + bounding_.addPos_.y });

		for (int32_t i = 0; i < kMaxDigit_; i++) {

			digitNum = judgeNumber / divideNum;

			numberSprite_[i]->SetTextureArea({ float(digitNum * 64.0f), 0.0f }, { 64.0f,64.0f });

			numberSprite_[i]->Draw();

			judgeNumber = judgeNumber % divideNum;

			divideNum = int32_t(divideNum * 0.1f);
		}


	}

}

void StageNumberDraw::AnimationInit() {
	bounding_.param_ = 0.0f;
}

void StageNumberDraw::AnimationUpdate() {

	const float step = std::numbers::pi_v<float> / (float)bounding_.cycle_;

	bounding_.param_ += step;

	bounding_.param_ = std::fmod(bounding_.param_, std::numbers::pi_v<float>);

	bounding_.addPos_.y = -std::sinf(bounding_.param_) * bounding_.amplitude_;
	
}
