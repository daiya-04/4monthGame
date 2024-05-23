#include "Option.h"
#include "TextureManager.h"
#include "Easing.h"
#include "Input.h"
#include "Audio.h"
#include "WinApp.h"


Option::Option() {
	
	windowTex_ = TextureManager::Load("OptionWindow.png");
	gaugeTex_ = TextureManager::Load("volumeGauge.png");
	gaugeFrameTex_ = TextureManager::Load("gaugeFrame.png");
	bgmTextImage_ = TextureManager::Load("BGM_Text.png");
	seTextImage_ = TextureManager::Load("SE_Text.png");
	fullScreenImage_ = TextureManager::Load("FullScreen_Text.png");
	checkBoxTex_ = TextureManager::Load("checkBox.png");
	checkBoxFrameTex_ = TextureManager::Load("checkBoxFrame.png");
	checkTex_ = TextureManager::Load("check.png");
	bButtonTex_ = TextureManager::Load("BButton.png");

#ifdef NDEBUG
	isFullScreen_ = true;
#endif // NDEBUG
#ifdef _DEBUG
	isFullScreen_ = false;
#endif // _DEBUG

	window_.reset(Sprite::Create(windowTex_, { 640.0f,360.0f }, 0.0f));

	bgmGauge_.reset(Sprite::Create(gaugeTex_, {640.0f - 150.0f, 260.0f}));
	bgmGauge_->SetAnchorpoint({ 0.0f,0.5f });
	bgmGauge_->SetScale({ bgmVolume_ * 0.1f,1.0f });

	seGauge_.reset(Sprite::Create(gaugeTex_, { 640.0f - 150.0f, 360.0f}));
	seGauge_->SetAnchorpoint({ 0.0f,0.5f });
	seGauge_->SetScale({ seVolume_ * 0.1f,1.0f });

	gaugeFrame_.reset(Sprite::Create(gaugeFrameTex_, { 640.0f,260.0f }));

	bgmText_.reset(Sprite::Create(bgmTextImage_, { 640.0f - 220.0f,260.0f }));
	seText_.reset(Sprite::Create(seTextImage_, { 640.0f - 220.0f,360.0f }));

	fullScreenText_.reset(Sprite::Create(fullScreenImage_, { 640.0f - 200.0f,460.0f }));

	checkBox_.reset(Sprite::Create(checkBoxTex_, { 640.0f,460.0f }));
	checkBoxFrame_.reset(Sprite::Create(checkBoxFrameTex_, { 640.0f,460.0f }));
	check_.reset(Sprite::Create(checkTex_, { 640.0f,460.0f }));

	bButton_.reset(Sprite::Create(bButtonTex_, { 640.0f + 240.0f,500.0f }));

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
		if (mode_ == Mode::Root) {
			bgmGauge_->Draw();
			seGauge_->Draw();
			bgmText_->Draw();
			seText_->Draw();
			fullScreenText_->Draw();
			checkBox_->Draw();
			bButton_->Draw();
			if (item_ != Item::FullScreen) {
				gaugeFrame_->Draw();
			}else {
				checkBoxFrame_->Draw();
			}
			if (isFullScreen_) {
				check_->Draw();
			}
		}
	}


}

void Option::RootInit() {

	

}

void Option::RootUpdate() {

	if (Input::GetInstance()->TriggerButton(Input::Button::B)) {
		modeRequest_ = Mode::Close;
	}

#ifdef _DEBUG
	if (Input::GetInstance()->TriggerKey(DIK_B)) {
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

	bgmGauge_->SetScale({ bgmVolume_ * 0.1f,1.0f });
	seGauge_->SetScale({ seVolume_ * 0.1f,1.0f });

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

	gaugeFrame_->SetPosition({ 640.0f,260.0f });

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

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		itemRequest_ = Item::SE;
	}
	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		bgmVolume_--;
	}else if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		bgmVolume_++;
	}

#endif // _DEBUG


	bgmVolume_ = std::clamp(bgmVolume_, 0u, 10u);

	Audio::bgmVolume_ = 0.1f * bgmVolume_;
}

void Option::SEInit() {
	gaugeFrame_->SetPosition({ 640.0f,360.0f });
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

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		itemRequest_ = Item::BGM;
	}
	if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		itemRequest_ = Item::FullScreen;
	}

	if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		seVolume_--;
	}
	if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		seVolume_++;
	}

#endif // _DEBUG


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
			isFullScreen_ = false;
			WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kWindow);
		}else if (!isFullScreen_) {
			isFullScreen_ = true;
			WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kFullScreen);
		}
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		itemRequest_ = Item::SE;
	}

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		if (isFullScreen_) {
			isFullScreen_ = false;
			WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kWindow);
		}
		else if (!isFullScreen_) {
			isFullScreen_ = true;
			WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kFullScreen);
		}
	}

#endif // _DEBUG



}
