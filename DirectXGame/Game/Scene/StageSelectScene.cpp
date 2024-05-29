#include "StageSelectScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Score/ScoreManager.h"
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
	playerTex_ = TextureManager::Load("player/playerBlueIdle.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

	player_.reset(Object2d::Create(playerTex_, { 282.0f,568.0f }));
	player_->SetSize({ 160.0f,160.0f });
	player_->SetTextureArea({ 0.0f,0.0f }, { 160.0f,160.0f });
	player_->SetScale({ -1.0f,1.0f });

	for (size_t index = 0; index < kMaxStage_; index++) {
		saunaRooms_.emplace_back(Object2d::Create(saunaRoomTex_, {640.0f + 640.0f * index, 653.0f}));
		saunaRooms_[index]->SetAnchorpoint({ 0.5f,1.0f });
		if (stageNumber_ == (index + 1)) {
			saunaRooms_[index]->SetScale(1.2f);
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

	uis_["BButton"]->SetScale(0.6f);

	///

	score_.Init(scorePos_, { 48.0f,48.0f });
	rank_.Init(rankPos_, { 64.0f,64.0f });

}

void StageSelectScene::Update() {
	DebugGUI();
	ApplyGlobalVariables();

	if (mode_ != Mode::Enter) {
		if (++animationTime_ >= changeFrame_) {

			animationTime_ = 0;

			if (++animationNum_ > 4) {
				animationNum_ = 0;
			}
		}
	}
	

	if (modeRequest_) {

		mode_ = modeRequest_.value();

		switch (mode_) {
			case Mode::Root:
				RootInit();
				break;
			case Mode::Move:
				MoveInit();
				break;
			case Mode::Enter:
				EnterInit();
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
		case Mode::Enter:
			EnterUpdate();
			break;
	}

	player_->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });

	camera_.UpdateViewMatrix();
}

void StageSelectScene::DrawBackGround() {

	backGround_->Draw();

}

void StageSelectScene::DrawObject() {

	if (mode_ == Mode::Enter) {
		player_->Draw(camera_);
	}
	for (const auto& room : saunaRooms_) {
		room->Draw(camera_);
	}
	if (mode_ != Mode::Enter) {
		player_->Draw(camera_);
	}
	

}

void StageSelectScene::DrawParticle() {



}

void StageSelectScene::DrawUI() {

	//stageNum_->Draw();
	for (const auto& ui : uis_) {
		ui.second->Draw();
	}

	if (mode_ == Mode::Root) {
		score_.Draw();
		rank_.Draw();
	}
	

}

void StageSelectScene::DebugGUI() {
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

void StageSelectScene::RootInit() {

	score_.SetScore(ScoreManager::GetInstance()->GetBestScore(stageNumber_ - 1));
	rank_.SetRank(ScoreManager::GetInstance()->GetBestRank(stageNumber_ - 1));

}

void StageSelectScene::RootUpdate() {

	preStageNum_ = stageNumber_;

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		modeRequest_ = Mode::Enter;
	}

	if (Input::GetInstance()->TriggerButton(Input::Button::B)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}

#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_1)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_3)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}
	if (Input::GetInstance()->TriggerKey(DIK_SPACE)) {
		//SceneManager::GetInstance()->ChangeScene("Game");
		modeRequest_ = Mode::Enter;
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

	float prePos = camera_.translation_.x;

	param_ += 0.03f;
	param_ = min(param_, 1.0f);

	float T = Easing::easeInOutQuart(param_);
	camera_.translation_.x = Lerp(T, moveStartPos_, moveEndPos_);
	
	float distance = camera_.translation_.x - prePos;
	player_->position_.x += distance;
	uint32_t index = stageNumber_ - 1;
	uint32_t preIndex = preStageNum_ - 1;

	if (index != preIndex) {
		saunaRooms_[index]->SetScale(Lerp(T, 0.3f, 1.2f));
		saunaRooms_[preIndex]->SetScale(Lerp(T, 1.2f, 0.3f));
	}

	if (param_ >= 1.0f) {
		modeRequest_ = Mode::Root;
		uis_["stageNumber"]->SetTextureArea({ 64.0f * stageNumber_, 64.0f * stageNumber_ }, { 64.0f,64.0f });
	}

}

void StageSelectScene::EnterInit() {

	param_ = 0.0f;

	moveStartPos_ = player_->position_.x;
	moveEndPos_ = moveStartPos_ + 400.0f;
	animationNum_ = 0;

}

void StageSelectScene::EnterUpdate() {

	param_ += 0.02f;
	param_ = min(param_, 1.0f);

	float T = Easing::easeInBack(param_);
	player_->position_.x = Lerp(T, moveStartPos_, moveEndPos_);

	if (param_ >= 1.0f) {
		SceneManager::GetInstance()->ChangeScene("Game");
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
