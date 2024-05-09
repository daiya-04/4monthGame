#include "TitleScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "GlobalVariables.h"
#include "Audio.h"
#include "Input.h"
#include <random>
#include <format>

TitleScene::~TitleScene() {}

void TitleScene::Init() {

	GlobalVariables* gb = GlobalVariables::GetInstance();
	
	//データの追加
	gb->CreateData(dataName);

	LoadData();

	bgTexture_ = TextureManager::Load("backGround/titleBG.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

}

void TitleScene::Update() {
	DebugGUI();
	ApplyGlobalVariables();

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		SceneManager::GetInstance()->ChangeScene("StageSelect");
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

	for (const auto& ui : uis_) {
		ui.second->Draw();
	}

}

void TitleScene::DebugGUI() {
#ifdef _DEBUG
	GlobalVariables* gb = GlobalVariables::GetInstance();

	ImGui::Begin("AddGroup");
	
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
	GlobalVariables* gb = GlobalVariables::GetInstance();

	for (const auto& ui : uis_) {
		ui.second->SetPosition(gb->GetValue<Vector2>(dataName, ui.first, "position"));
	}

}
