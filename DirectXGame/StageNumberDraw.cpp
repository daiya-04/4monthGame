#include "StageNumberDraw.h"
#include "TextureManager.h"

void StageNumberDraw::Init(const Vector2& pos, const Vector2& size) {

	uint32_t numberTex = TextureManager::Load("UI/number.png");

	position_ = pos;
	size_ = size;

	for (size_t index = 0; index < kMaxDigit_; index++) {
		numberSprite_[index].reset(Sprite::Create(numberTex, {}));
		numberSprite_[index]->SetSize(size_);
	}

}

void StageNumberDraw::Draw(int32_t stageNumber) {

	if (stageNumber < 10) {
		numberSprite_[0]->SetPosition(position_);
		numberSprite_[0]->SetTextureArea({ 64.0f * stageNumber,0.0f }, { 64.0f,64.0f });
		numberSprite_[0]->Draw();
	}
	else {

		int32_t digitNum = 0;

		int32_t divideNum = int32_t(std::pow(10, kMaxDigit_ - 1));

		int32_t judgeNumber = stageNumber;

		numberSprite_[0]->SetPosition({ position_.x - 25.0f, position_.y });
		numberSprite_[1]->SetPosition({ position_.x + 25.0f, position_.y });

		for (int32_t i = 0; i < kMaxDigit_; i++) {

			digitNum = judgeNumber / divideNum;

			numberSprite_[i]->SetTextureArea({ float(digitNum * 64.0f), 0.0f }, { 64.0f,64.0f });

			numberSprite_[i]->Draw();

			judgeNumber = judgeNumber % divideNum;

			divideNum = int32_t(divideNum * 0.1f);
		}


	}

}
