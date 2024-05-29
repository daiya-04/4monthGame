#include "TitleAnimation.h"
#include "TextureManager.h"

void TitleAnimation::Init() {

	saunaRoomTex_ = TextureManager::Load("saunaRoom.png");
	playerTexes_[Blue] = TextureManager::Load("player/playerBlueRun.png");
	playerTexes_[Orange] = TextureManager::Load("player/playerOrangeRun.png");

	for (size_t index = 0; index < 2; index++) {
		players_[index].reset(Sprite::Create(playerTexes_[index], {}));
		players_[index]->SetAnchorpoint({ 0.5f,1.0f });
		players_[index]->SetSize({ 160.0f,160.0f });
		players_[index]->SetTextureArea({ 0.0f,0.0f }, { 160.0f,160.0f });
		players_[index]->SetScale(playerScale_);
	}

	saunaRoom_.reset(Sprite::Create(saunaRoomTex_, { 640.0f,725.0f }));
	saunaRoom_->SetAnchorpoint({ 0.5f,1.0f });
	saunaRoom_->SetScale(1.3f);

	modeRequest_ = Mode::BlueIn;

}

void TitleAnimation::Update() {

	

	if (modeRequest_) {

		mode_ = modeRequest_.value();

		switch (mode_) {
			case Mode::BlueIn:
				BlueInInit();
				break;
			case Mode::OrangeOut:
				OrangeOutInit();
				break;
			case Mode::OrangeIn:
				OrangeInInit();
				break;
			case Mode::BlueOut:
				BlueOutInit();
				break;
		}

		modeRequest_ = std::nullopt;
	}

	if (++animationTime_ >= changeFrame_) {

		animationTime_ = 0;

		if (++animationNum_ > 8) {
			animationNum_ = 0;
		}
	}

	switch (mode_) {
		case Mode::BlueIn:
			BlueInUpdate();
			break;
		case Mode::OrangeOut:
			OrangeOutUpdate();
			break;
		case Mode::OrangeIn:
			OrangeInUpdate();
			break;
		case Mode::BlueOut:
			BlueOutUpdate();
			break;
	}

	for (size_t index = 0; index < 2; index++) {
		players_[index]->SetPosition(playerPos_[index]);
	}
	

}

void TitleAnimation::Draw() {

	players_[Blue]->Draw();
	players_[Orange]->Draw();
	saunaRoom_->Draw();

}

void TitleAnimation::BlueInInit() {

	playerPos_[Blue] = { kBlueInPos_, kPlayerYPos_ };
	players_[Blue]->SetScale({ -playerScale_,playerScale_ });
	AnimationFrameInit();

}

void TitleAnimation::BlueInUpdate() {

	playerPos_[Blue].x += kSpeed_;

	if (playerPos_[Blue].x >= kCenterPos_) {
		playerPos_[Blue].x = kCenterPos_;
		modeRequest_ = Mode::OrangeOut;
	}

	players_[Blue]->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });
}

void TitleAnimation::OrangeOutInit() {

	playerPos_[Orange] = { kCenterPos_, kPlayerYPos_ };
	players_[Orange]->SetScale({ -playerScale_,playerScale_ });
	AnimationFrameInit();

}

void TitleAnimation::OrangeOutUpdate() {

	playerPos_[Orange].x += kSpeed_;

	if (playerPos_[Orange].x >= kOrangeInPos_) {
		playerPos_[Orange].x = kOrangeInPos_;
		modeRequest_ = Mode::OrangeIn;
	}

	players_[Orange]->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });
}

void TitleAnimation::OrangeInInit() {

	playerPos_[Orange] = { kOrangeInPos_,kPlayerYPos_ };
	players_[Orange]->SetScale({ playerScale_,playerScale_ });
	AnimationFrameInit();

}

void TitleAnimation::OrangeInUpdate() {

	playerPos_[Orange].x -= kSpeed_;

	if (playerPos_[Orange].x <= kCenterPos_) {
		playerPos_[Orange].x = kCenterPos_;
		modeRequest_ = Mode::BlueOut;
	}

	players_[Orange]->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });
}

void TitleAnimation::BlueOutInit() {

	playerPos_[Blue] = { kCenterPos_,kPlayerYPos_ };
	players_[Blue]->SetScale({ playerScale_,playerScale_ });
	AnimationFrameInit();

}

void TitleAnimation::BlueOutUpdate() {

	playerPos_[Blue].x -= kSpeed_;

	if (playerPos_[Blue].x <= kBlueInPos_) {
		playerPos_[Blue].x = kBlueInPos_;
		modeRequest_ = Mode::BlueIn;
	}

	players_[Blue]->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });
}

void TitleAnimation::AnimationFrameInit() {
	animationNum_ = 0;
	animationTime_ = 0;
}
