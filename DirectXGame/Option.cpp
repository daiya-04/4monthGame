#include "Option.h"
#include "TextureManager.h"
#include "Easing.h"
#include "Input.h"
#include "Audio.h"
#include "WinApp.h"


Option::Option() {
	
	windowTex_ = TextureManager::Load("OptionWindow.png");



	window_.reset(Sprite::Create(windowTex_, { 640.0f,360.0f }, 0.0f));

}

void Option::Init() {

	isWindow_ = true;
	modeRequest_ = Mode::Open;
	itemRequest_ = Item::BGM;

}

void Option::Update() {

	if (!isWindow_) { return; }

	if (modeRequest_) {

		mode_ = modeRequest_.value();

		switch (mode_) {
			case Mode::Root:
				RootInit();
				break;
			case Mode::Open:
				OpenInit();
				break;
			case Mode::Close:
				CloseInit();
				break;
		}

		modeRequest_ = std::nullopt;
	}

	switch (mode_) {
		case Mode::Root:
			RootUpdate();
			break;
		case Mode::Open:
			OpenUpdate();
			break;
		case Mode::Close:
			CloseUpdate();
			break;
	}


}

void Option::Draw() {

	if (isWindow_) {
		window_->Draw();
	}


}

void Option::RootInit() {

	

}

void Option::RootUpdate() {

	if (Input::GetInstance()->TriggerButton(Input::Button::B)) {
		modeRequest_ = Mode::Close;
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		modeRequest_ = Mode::Close;
	}
#endif // _DEBUG

	if (itemRequest_) {

		item_ = itemRequest_.value();

		switch (item_) {
			case Item::BGM:
				BGMInit();
				break;
			case Item::SE:
				SEInit();
				break;
			case Item::FullScreen:
				FSInit();
				break;
		}

		itemRequest_ = std::nullopt;
	}

	switch (item_) {
		case Item::BGM:
			BGMUpdate();
			break;
		case Item::SE:
			SEUpdate();
			break;
		case Item::FullScreen:
			FSUpdate();
			break;
	}

}

void Option::OpenInit() {

	param_ = 0.0f;

}

void Option::OpenUpdate() {

	param_ += 0.03f;
	param_ = min(param_, 1.0f);

	float wT = Easing::easeOutQuart(param_);
	float hT = Easing::easeInQuart(param_);

	window_->SetScale({ Lerp(wT,0.0f,1.0f),Lerp(hT,0.0f,1.0f) });

	if (param_ >= 1.0f) {
		modeRequest_ = Mode::Root;
	}

}

void Option::CloseInit() {

	param_ = 0.0f;

}

void Option::CloseUpdate() {

	param_ += 0.03f;
	param_ = min(param_, 1.0f);

	float wT = Easing::easeInQuart(param_);
	float hT = Easing::easeOutQuart(param_);

	window_->SetScale({ Lerp(wT,1.0f,0.0f),Lerp(hT,1.0f,0.0f) });

	if (param_ >= 1.0f) {
		isWindow_ = false;
	}

}

void Option::BGMInit() {

}

void Option::BGMUpdate() {

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Down) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN)) {
		itemRequest_ = Item::SE;
	}

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Left) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_LEFT)) {
		bgmVolume_--;
	}
	else if(Input::GetInstance()->TriggerLStick(Input::Stick::Right) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_RIGHT)){
		bgmVolume_++;
	}
	bgmVolume_ = std::clamp(bgmVolume_, 0u, 10u);

	Audio::bgmVolume_ = 0.1f * bgmVolume_;
}

void Option::SEInit() {

}

void Option::SEUpdate() {

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Up) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP)) {
		itemRequest_ = Item::BGM;
	}
	if (Input::GetInstance()->TriggerLStick(Input::Stick::Down) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN)) {
		itemRequest_ = Item::FullScreen;
	}

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Left) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_LEFT)) {
		seVolume_--;
	}
	else if (Input::GetInstance()->TriggerLStick(Input::Stick::Right) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_RIGHT)) {
		seVolume_++;
	}
	seVolume_ = std::clamp(seVolume_, 0u, 10u);

	Audio::seVolume_ = 0.1f * seVolume_;
}

void Option::FSInit() {

}

void Option::FSUpdate() {

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Up) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP)) {
		itemRequest_ = Item::SE;
	}

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		if (isFullScreen_) {
			WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kWindow);
		}else if (!isFullScreen_) {
			WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kFullScreen);
		}
	}


}
