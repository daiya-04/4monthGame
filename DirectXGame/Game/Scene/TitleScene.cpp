#include "TitleScene.h"

#include "WinApp.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "GlobalVariables.h"
#include "AudioManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>
#include <format>
#include <numbers>

TitleScene::~TitleScene() {
	titleBGM_->StopSound();
}

void TitleScene::Init() {

	camera_.Init();

	GlobalVariables* gb = GlobalVariables::GetInstance();
	
	//データの追加
	gb->CreateData(dataName);

	LoadData();

	bgTexture_ = TextureManager::Load("backGround/titleBG.png");
	circleTex_ = TextureManager::Load("circle.png");
	

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

	option_ = std::make_unique<Option>();

	circle_.reset(Sprite::Create(circleTex_, {}));

	uis_["Arrow"]->SetSize({ 64.0f,64.0f });
	uis_["Arrow"]->SetTextureArea({ 64.0f,64.0f }, { 64.0f,64.0f });

	if (isOpening_) {
		uis_["Arrow"]->DrawOff();
		uis_["Exit"]->DrawOff();
		uis_["Start"]->DrawOff();
		uis_["Option"]->DrawOff();
	}

	titleAnima_ = std::make_unique<TitleAnimation>();
	titleAnima_->Init();

	circle_->SetPosition(uis_["AButton"]->GetPosition());

	stageNumber_ = 1;

	selectSE_ = AudioManager::GetInstance()->Load("SE/select_ok.mp3");
	moveSE_ = AudioManager::GetInstance()->Load("SE/select_move.mp3");
	cancelSE_ = AudioManager::GetInstance()->Load("SE/select_cancel.mp3");
	titleBGM_ = AudioManager::GetInstance()->Load("BGM/titleBGM.mp3");

	titleBGM_->Play();

	steam_ = std::make_unique<Steam>();
	steam_->Init({ 640.0f,360.0f }, { 640.0f,360.0f });
	steam_->SetEmitCount(2);

	FloatingGimmickInit();
	ButtonEffectInit();
}

void TitleScene::Update() {
	DebugGUI();
	ApplyGlobalVariables();

	if (selectRequest_) {

		select_ = selectRequest_.value();

		switch (select_) {
			case Select::Start:
				StartInit();
				break;
			case Select::Option:
				OptionInit();
				break;
			case Select::Exit:
				ExitInit();
				break;
		}

		selectRequest_ = std::nullopt;
	}

	switch (select_) {
		case Select::Start:
			StartUpdate();
			break;
		case Select::Option:
			OptionUpdate();
			break;
		case Select::Exit:
			ExitUpdate();
			break;
	}
	
	FloatingGimmickUpdate();
	if (isOpening_) {
		ButtonEffectUpdate();
	}
	
	titleAnima_->Update();
	steam_->Update();

#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_2)) {
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_3)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

#endif // _DEBUG

	camera_.UpdateViewMatrix();
}

void TitleScene::DrawBackGround() {

	backGround_->Draw();

	Particle::preDraw();
	steam_->Draw(camera_);

}

void TitleScene::DrawObject() {



}

void TitleScene::DrawParticle() {



}

void TitleScene::DrawUI() {

	titleAnima_->Draw();

	if (!option_->IsWindow()) {
		circle_->Draw();
		for (const auto& ui : uis_) {
			ui.second->Draw();
		}
	}

	option_->Draw();

}

void TitleScene::StartInit() {

	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Start"]->GetPosition().y });
	BoundingInit(uis_["Start"]->GetPosition());

}

void TitleScene::StartUpdate() {

#ifdef _DEBUG
	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Start"]->GetPosition().y });
#endif // _DEBUG

	BoundingUpdate(uis_["Start"].get());

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		if (isOpening_) {
			isOpening_ = false;
			uis_["Arrow"]->DrawOn();
			uis_["Exit"]->DrawOn();
			uis_["Start"]->DrawOn();
			uis_["Option"]->DrawOn();
			uis_["AButton"]->DrawOff();
			circle_->DrawOff();
			selectSE_->Play();
		}else {
			SceneManager::GetInstance()->ChangeScene("StageSelect");
			selectSE_->Play();
		}
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		if (isOpening_) {
			isOpening_ = false;
			uis_["Arrow"]->DrawOn();
			uis_["Exit"]->DrawOn();
			uis_["Start"]->DrawOn();
			uis_["Option"]->DrawOn();
			uis_["AButton"]->DrawOff();
			circle_->DrawOff();
			selectSE_->Play();
		}
		else {
			SceneManager::GetInstance()->ChangeScene("StageSelect");
			selectSE_->Play();
		}
	}

#endif // _DEBUG

	if (isOpening_) { return; }

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Down) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN)) {
		selectRequest_ = Select::Option;
		moveSE_->Play();
		uis_["Start"]->SetPosition(workBounding_.axisPos_);
	}

#ifdef _DEBUG
	
	if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		selectRequest_ = Select::Option;
		moveSE_->Play();
	}

#endif // _DEBUG
	

}

void TitleScene::OptionInit() {

	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Option"]->GetPosition().y });
	BoundingInit(uis_["Option"]->GetPosition());

}

void TitleScene::OptionUpdate() {

#ifdef _DEBUG
	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Option"]->GetPosition().y });
#endif // _DEBUG

	BoundingUpdate(uis_["Option"].get());
	

	if (!option_->IsWindow()) {

		if (Input::GetInstance()->TriggerLStick(Input::Stick::Up) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP)) {
			selectRequest_ = Select::Start;
			moveSE_->Play();
			uis_["Option"]->SetPosition(workBounding_.axisPos_);
		}

		if (Input::GetInstance()->TriggerLStick(Input::Stick::Down) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN)) {
			selectRequest_ = Select::Exit;
			moveSE_->Play();
			uis_["Option"]->SetPosition(workBounding_.axisPos_);
		}

		if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
			option_->Init();
			selectSE_->Play();
		}

#ifdef _DEBUG

		if (Input::GetInstance()->TriggerKey(DIK_UP)) {
			selectRequest_ = Select::Start;
			moveSE_->Play();
		}
		if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
			selectRequest_ = Select::Exit;
			moveSE_->Play();
		}
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			option_->Init();
			selectSE_->Play();
		}

#endif // _DEBUG
	}

	option_->Update();
	

}

void TitleScene::ExitInit() {

	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Exit"]->GetPosition().y });
	BoundingInit(uis_["Exit"]->GetPosition());

}

void TitleScene::ExitUpdate() {

#ifdef _DEBUG
	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Exit"]->GetPosition().y });
#endif // _DEBUG

	BoundingUpdate(uis_["Exit"].get());

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		//cancelSE_->Play();
		titleBGM_->StopSound();
		WinApp::GetInstance()->GameEnd();
	}

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Up) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP)) {
		selectRequest_ = Select::Option;
		moveSE_->Play();
		uis_["Exit"]->SetPosition(workBounding_.axisPos_);
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		titleBGM_->StopSound();
		WinApp::GetInstance()->GameEnd();
	}
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		selectRequest_ = Select::Option;
		moveSE_->Play();
	}

#endif // _DEBUG

}

void TitleScene::DebugGUI() {
#ifdef _DEBUG
	GlobalVariables* gb = GlobalVariables::GetInstance();

	ImGui::Begin("AddUI");
	
	ImGui::InputText("texurePath", filePathBuff, sizeof(filePathBuff));
	std::string filePath = filePathBuff;
	ImGui::InputText("groupName", groupNameBuff, sizeof(groupNameBuff));
	std::string groupName = groupNameBuff;
	
	ImGui::InputFloat2("AddPos", &addPos.x);

	if (ImGui::Button("Add")) {
		

		gb->CreateGroup(dataName, groupName);

		gb->AddItem<const std::string&>(dataName, groupName, "filePath", filePath);
		gb->AddItem<const Vector2&>(dataName, groupName, "position", addPos);

		gb->SaveFile(dataName);
		LoadData();
		std::string message = std::format("{}.Add", groupName);
		MessageBoxA(nullptr, message.c_str(), "GlobalVariables", 0);

	}

	ImGui::End();

#endif // _DEBUG
}

void TitleScene::LoadData() {
	GlobalVariables* gb = GlobalVariables::GetInstance();

	for (size_t i = 0; i < gb->GetGroupNum(dataName); i++) {

		std::string groupName = gb->GetGroupName(dataName, i);

		if (uis_.find(groupName) == uis_.end()) {
			uint32_t texture = TextureManager::Load(gb->GetValue<std::string>(dataName, groupName, "filePath"));
			uis_.emplace(groupName, Sprite::Create(texture, gb->GetValue<Vector2>(dataName, groupName, "position")));
		}
	}

}

void TitleScene::ApplyGlobalVariables() {
#ifdef _DEBUG

	GlobalVariables* gb = GlobalVariables::GetInstance();

	for (const auto& ui : uis_) {
		ui.second->SetPosition(gb->GetValue<Vector2>(dataName, ui.first, "position"));
	}

	workFloating_.axisPos_ = uis_["TitleLogo"]->GetPosition();

#endif // _DEBUG
}

void TitleScene::FloatingGimmickInit() {

	workFloating_.param_ = 0.0f;
	workFloating_.axisPos_ = uis_["TitleLogo"]->GetPosition();

}

void TitleScene::FloatingGimmickUpdate() {

	const float step = 2.0f * std::numbers::pi_v<float> / (float)workFloating_.cycle_;

	workFloating_.param_ += step;

	workFloating_.param_ = std::fmod(workFloating_.param_, 2.0f * std::numbers::pi_v<float>);
	
	workFloating_.addPos_.y += std::sinf(workFloating_.param_) * workFloating_.amplitude_;

	uis_["TitleLogo"]->SetPosition(workFloating_.axisPos_ + workFloating_.addPos_);
}

void TitleScene::ButtonEffectInit() {

	buttonEffect_.param_ = 0.0f;
	buttonEffect_.alpha_ = 1.0f;

}

void TitleScene::ButtonEffectUpdate() {

	buttonEffect_.param_ += 0.04f;

	buttonEffect_.param_ = std::fmod(buttonEffect_.param_, 1.0f);

	buttonEffect_.alpha_ = Lerp(buttonEffect_.param_, 1.0f, 0.0f);
	buttonEffect_.scale_ = Lerp(buttonEffect_.param_, 0.9f, 1.2f);

	circle_->SetColor({ 1.0f,1.0f,1.0f,buttonEffect_.alpha_ });
	circle_->SetScale(buttonEffect_.scale_);
}

void TitleScene::BoundingInit(const Vector2& axisPos) {

	workBounding_.axisPos_ = axisPos;
	workBounding_.velocity_.y = -1.7f;
	workBounding_.accel_ = {};
	workBounding_.addPos_ = {};

}

void TitleScene::BoundingUpdate(Sprite* sprite) {

	workBounding_.accel_.y += 0.02f;

	workBounding_.velocity_.y += workBounding_.accel_.y;

	workBounding_.addPos_ += workBounding_.velocity_;

	if (workBounding_.addPos_.y > 0) {
		BoundingInit(workBounding_.axisPos_);
	}

	sprite->SetPosition(workBounding_.axisPos_ + workBounding_.addPos_);

}
