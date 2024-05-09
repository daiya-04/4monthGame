#include "StageSelectScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "GlobalVariables.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include "Easing.h"
#include <random>
#include <format>

StageSelectScene::~StageSelectScene() {}

void StageSelectScene::Init() {

	GlobalVariables* gb = GlobalVariables::GetInstance();
	
	//データの追加
	gb->CreateData(dataName);

	LoadData();

	camera_.Init();
	camera_.translation_ = { 0.0f,0.0f,0.0f };

	bgTexture_ = TextureManager::Load("backGround/stageSelectBG.png");
	saunaRoomTex_ = TextureManager::Load("saunaRoom.png");
	//stageNumTex_ = TextureManager::Load("stageNumber.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

	//stageNum_.reset(Sprite::Create(stageNumTex_, { 640.0f,400.0f }));
	//stageNum_->SetSize({ 96.0f, 96.0f });
	//stageNum_->SetTextureArea({ 0.0f,0.0f }, { 32.0f,32.0f });

	for (size_t index = 0; index < kMaxStage_; index++) {
		saunaRooms_.emplace_back(Object2d::Create(saunaRoomTex_, {640.0f + 640.0f * index, 650.0f}));
		saunaRooms_[index]->SetAnchorpoint({ 0.5f,1.0f });
		if (stageNumber_ == (index + 1)) {
			saunaRooms_[index]->SetScale(1.0f);
		}
		else {
			saunaRooms_[index]->SetScale(0.3f);
		}
	}

	///UIの設定

	uis_["stageNumber"]->SetSize({ 64.0f,64.0f });
	uis_["stageNumber"]->SetTextureArea({ 64.0f * stageNumber_, 64.0f * stageNumber_ }, { 64.0f,64.0f });

	uis_["RArrow"]->SetSize({ 64.0f,64.0f });
	uis_["RArrow"]->SetTextureArea({ 64.0f,64.0f }, { 64.0f,64.0f });

	uis_["LArrow"]->SetSize({ 64.0f,64.0f });
	uis_["LArrow"]->SetTextureArea({}, { 64.0f,64.0f });

	///

}

void StageSelectScene::Update() {
	DebugGUI();
	ApplyGlobalVariables();

	if (modeRequest_) {

		mode_ = modeRequest_.value();

		switch (mode_) {
			case Mode::Root:
				RootInit();
				break;
			case Mode::Move:
				MoveInit();
				break;
		}

		modeRequest_ = std::nullopt;
	}

	switch (mode_) {
		case Mode::Root:
			RootUpdate();
			break;
		case Mode::Move:
			MoveUpdate();
			break;
	}

	camera_.UpdateViewMatrix();
}

void StageSelectScene::DrawBackGround() {

	backGround_->Draw();

}

void StageSelectScene::DrawObject() {

	for (const auto& room : saunaRooms_) {
		room->Draw(camera_);
	}

}

void StageSelectScene::DrawParticle() {



}

void StageSelectScene::DrawUI() {

	//stageNum_->Draw();
	for (const auto& ui : uis_) {
		ui.second->Draw();
	}

}

void StageSelectScene::DebugGUI() {
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

void StageSelectScene::RootInit() {



}

void StageSelectScene::RootUpdate() {

	preStageNum_ = stageNumber_;

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_1)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_3)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

#endif // _DEBUG


	if (Input::GetInstance()->TriggerLStick(Input::Stick::Right) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_RIGHT)) {
		stageNumber_++;
		modeRequest_ = Mode::Move;
	}
	else if (Input::GetInstance()->TriggerLStick(Input::Stick::Left) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_LEFT)) {
		stageNumber_--;
		modeRequest_ = Mode::Move;
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		stageNumber_++;
		modeRequest_ = Mode::Move;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		stageNumber_--;
		modeRequest_ = Mode::Move;
	}

#endif // _DEBUG

	stageNumber_ = max(1, min(stageNumber_, kMaxStage_));

	

}

void StageSelectScene::MoveInit() {

	param_ = 0.0f;

	moveStartPos_ = 640.0f * (preStageNum_ - 1);
	moveEndPos_ = 640.0f * (stageNumber_ - 1);

}

void StageSelectScene::MoveUpdate() {

	param_ += 0.03f;
	param_ = min(param_, 1.0f);

	float T = Easing::easeInOutQuart(param_);
	camera_.translation_.x = Lerp(T, moveStartPos_, moveEndPos_);

	uint32_t index = stageNumber_ - 1;
	uint32_t preIndex = preStageNum_ - 1;

	if (index != preIndex) {
		saunaRooms_[index]->SetScale(Lerp(T, 0.3f, 1.0f));
		saunaRooms_[preIndex]->SetScale(Lerp(T, 1.0f, 0.3f));
	}
	

	/*if (stageNumber_ < preStageNum_) {

		

	}else if (stageNumber_ > preStageNum_) {


		

	}*/


	if (param_ >= 1.0f) {
		modeRequest_ = Mode::Root;
		uis_["stageNumber"]->SetTextureArea({ 64.0f * stageNumber_, 64.0f * stageNumber_ }, { 64.0f,64.0f });
	}

}

void StageSelectScene::LoadData() {
	GlobalVariables* gb = GlobalVariables::GetInstance();

	for (size_t i = 0; i < gb->GetGroupNum(dataName); i++) {

		std::string groupName = gb->GetGroupName(dataName, i);

		if (uis_.find(groupName) == uis_.end()) {
			uint32_t texture = TextureManager::Load(gb->GetValue<std::string>(dataName, groupName, "filePath"));
			uis_.emplace(groupName, Sprite::Create(texture, gb->GetValue<Vector2>(dataName, groupName, "position")));
		}
	}

}

void StageSelectScene::ApplyGlobalVariables() {
	GlobalVariables* gb = GlobalVariables::GetInstance();

	for (const auto& ui : uis_) {
		ui.second->SetPosition(gb->GetValue<Vector2>(dataName, ui.first, "position"));
	}

}
