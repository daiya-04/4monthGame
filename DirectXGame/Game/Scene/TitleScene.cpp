#include "TitleScene.h"

#include "WinApp.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "GlobalVariables.h"
#include "Audio.h"
#include "Input.h"
#include <random>
#include <format>
#include <numbers>

TitleScene::~TitleScene() {}

void TitleScene::Init() {

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

	titlePos_ = uis_["TitleLogo"]->GetPosition();

	circle_->SetPosition(uis_["AButton"]->GetPosition());

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
	

#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_2)) {
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_3)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

#endif // _DEBUG

}

void TitleScene::DrawBackGround() {

	backGround_->Draw();

}

void TitleScene::DrawObject() {



}

void TitleScene::DrawParticle() {



}

void TitleScene::DrawUI() {

	if (!option_->IsWindow()) {
		circle_->Draw();
		for (const auto& ui : uis_) {
			ui.second->Draw();
		}
	}

	option_->Draw();

}

void TitleScene::StartInit() {

	

}

void TitleScene::StartUpdate() {

	

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		if (isOpening_) {
			isOpening_ = false;
			uis_["Arrow"]->DrawOn();
			uis_["Exit"]->DrawOn();
			uis_["Start"]->DrawOn();
			uis_["Option"]->DrawOn();
			uis_["AButton"]->DrawOff();
			circle_->DrawOff();
		}else {
			SceneManager::GetInstance()->ChangeScene("StageSelect");
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
		}
		else {
			SceneManager::GetInstance()->ChangeScene("StageSelect");
		}
	}

#endif // _DEBUG

	if (isOpening_) { return; }

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Down) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN)) {
		selectRequest_ = Select::Option;
	}

#ifdef _DEBUG
	
	if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
		selectRequest_ = Select::Option;
	}

#endif // _DEBUG

	

	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Start"]->GetPosition().y });

}

void TitleScene::OptionInit() {



}

void TitleScene::OptionUpdate() {

	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Option"]->GetPosition().y });

	if (!option_->IsWindow()) {

		if (Input::GetInstance()->TriggerLStick(Input::Stick::Up) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP)) {
			selectRequest_ = Select::Start;
		}

		if (Input::GetInstance()->TriggerLStick(Input::Stick::Down) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN)) {
			selectRequest_ = Select::Exit;
		}

		if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
			option_->Init();
		}

#ifdef _DEBUG

		if (Input::GetInstance()->TriggerKey(DIK_UP)) {
			selectRequest_ = Select::Start;
		}
		if (Input::GetInstance()->TriggerKey(DIK_DOWN)) {
			selectRequest_ = Select::Exit;
		}
		if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
			option_->Init();
		}

#endif // _DEBUG
	}

	option_->Update();
	

}

void TitleScene::ExitInit() {

	

}

void TitleScene::ExitUpdate() {

	uis_["Arrow"]->SetPosition({ uis_["Arrow"]->GetPosition().x,uis_["Exit"]->GetPosition().y });

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		WinApp::GetInstance()->GameEnd();
	}

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Up) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP)) {
		selectRequest_ = Select::Option;
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		WinApp::GetInstance()->GameEnd();
	}
	if (Input::GetInstance()->TriggerKey(DIK_UP)) {
		selectRequest_ = Select::Option;
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

#endif // _DEBUG
}

void TitleScene::FloatingGimmickInit() {

	workFloating_.param_ = 0.0f;

}

void TitleScene::FloatingGimmickUpdate() {

	const float step = 2.0f * std::numbers::pi_v<float> / (float)workFloating_.cycle_;

	workFloating_.param_ += step;

	workFloating_.param_ = std::fmod(workFloating_.param_, 2.0f * std::numbers::pi_v<float>);

	titlePos_.y += std::sinf(workFloating_.param_) * workFloating_.amplitude_;

	uis_["TitleLogo"]->SetPosition(titlePos_);
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
