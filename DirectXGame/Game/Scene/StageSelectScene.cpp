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
#include <numbers>

StageSelectScene::~StageSelectScene() {
	selectBGM_->StopSound();
}

void StageSelectScene::Init() {

	GlobalVariables* gb = GlobalVariables::GetInstance();
	
	//データの追加
	gb->CreateData(dataName);

	LoadData();

	camera_.Init();
	camera_.translation_ = { 640.0f * (stageNumber_ - 1),0.0f,0.0f };

	bgTexture_ = TextureManager::Load("backGround/stageSelectBG.png");
	saunaRoomTex_ = TextureManager::Load("saunaRoom.png");
	playerTex_[Blue] = TextureManager::Load("player/playerBlueIdle.png");
	playerTex_[Orange] = TextureManager::Load("player/playerOrangeIdle.png");
	playerRunTex_[Blue] = TextureManager::Load("player/playerBlueRun.png");
	playerRunTex_[Orange] = TextureManager::Load("player/playerOrangeRun.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

	for (size_t index = 0; index < 2; index++) {
		player_[index].reset(Object2d::Create(playerTex_[index], {}));
		player_[index]->SetSize({ 160.0f,160.0f });
		player_[index]->SetTextureArea({ 0.0f,0.0f }, { 160.0f,160.0f });
		player_[index]->SetScale({ -1.0f,1.0f });
	}

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

	player_[Blue]->position_ = { saunaRooms_[stageNumber_ - 1]->position_.x - 400.0f, 568.0f};
	player_[Orange]->position_ = { saunaRooms_[stageNumber_ - 1]->position_.x - 500.0f,568.0f };

	stageNumberDraw_ = std::make_unique<StageNumberDraw>();
	stageNumberDraw_->Init({ 640.0f,270.0f }, { 96.0f, 96.0f });
	stageNumberDraw_->SetStageNumber(stageNumber_);

	///UIの設定

	uis_["RArrow"]->SetSize({ 64.0f,64.0f });
	uis_["RArrow"]->SetTextureArea({ 64.0f,64.0f }, { 64.0f,64.0f });

	uis_["LArrow"]->SetSize({ 64.0f,64.0f });
	uis_["LArrow"]->SetTextureArea({}, { 64.0f,64.0f });

	uis_["BButton"]->SetScale(0.6f);

	///

	score_.Init(scorePos_, { 48.0f,48.0f });

	score_.SetSpace(32.0f);
	rank_.Init(rankPos_, { 64.0f,64.0f });

	selectBGM_ = AudioManager::GetInstance()->Load("BGM/selectBGM.mp3");

	selectSE_ = AudioManager::GetInstance()->Load("SE/select_ok.mp3");
	moveSE_ = AudioManager::GetInstance()->Load("SE/select_move.mp3");
	cancelSE_ = AudioManager::GetInstance()->Load("SE/select_cancel.mp3");

	selectBGM_->Play();

}

void StageSelectScene::Update() {
	DebugGUI();
	ApplyGlobalVariables();

	if (++animationTime_ >= changeFrame_) {

		animationTime_ = 0;

		if (++animationNum_ > 4) {
			animationNum_ = 0;
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

	

	player_[Blue]->SetTextureArea({160.0f * animationNum_,160.0f}, {160.0f,160.0f});
	player_[Orange]->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });

	camera_.UpdateViewMatrix();
}

void StageSelectScene::DrawBackGround() {

	backGround_->Draw();

}

void StageSelectScene::DrawObject() {

	if (mode_ == Mode::Enter) {
		player_[Blue]->Draw(camera_);
		player_[Orange]->Draw(camera_);
	}
	for (const auto& room : saunaRooms_) {
		room->Draw(camera_);
	}
	if (mode_ != Mode::Enter) {
		player_[Blue]->Draw(camera_);
		player_[Orange]->Draw(camera_);
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
		stageNumberDraw_->Draw();
		score_.Draw();
		rank_.Draw();
	}
	

}

void StageSelectScene::DebugGUI() {
#ifdef _DEBUG

	GlobalVariables* gb = GlobalVariables::GetInstance();

	ImGui::Begin("AddUI");

	ImGui::InputText("texturePath", filePathBuff, sizeof(filePathBuff));
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

	uis_["RArrow"]->DrawOn();
	uis_["LArrow"]->DrawOn();

	if (stageNumber_ == 1) {
		uis_["LArrow"]->DrawOff();
	}
	if (stageNumber_ == kMaxStage_) {
		uis_["RArrow"]->DrawOff();
	}

	stageNumberDraw_->SetStageNumber(stageNumber_);
	stageNumberDraw_->AnimationInit();

	arrowFloating_.param_ = 0.0f;

	LArrowPos_ = uis_["LArrow"]->GetPosition();
	RArrowPos_ = uis_["RArrow"]->GetPosition();

}

void StageSelectScene::RootUpdate() {

	preStageNum_ = stageNumber_;

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		selectSE_->Play();
		modeRequest_ = Mode::Enter;
	}

	if (Input::GetInstance()->TriggerButton(Input::Button::B)) {
		cancelSE_->Play();
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
		if (stageNumber_ != kMaxStage_) {
			stageNumber_++;
			moveSE_->Play();
			modeRequest_ = Mode::Move;
		}
	}
	else if (Input::GetInstance()->TriggerLStick(Input::Stick::Left) || Input::GetInstance()->TriggerButton(Input::Button::DPAD_LEFT)) {
		if (stageNumber_ != 1) {
			stageNumber_--;
			moveSE_->Play();
			modeRequest_ = Mode::Move;
		}
	}

	if (Input::GetInstance()->TiltLStick(Input::Stick::Right) || Input::GetInstance()->PushButton(Input::Button::DPAD_RIGHT)) {
		if (++intervalCount_ >= moveInterval_) {
			if (stageNumber_ != kMaxStage_) {
				stageNumber_++;
				modeRequest_ = Mode::Move;
			}
		}
	}
	else if (Input::GetInstance()->TiltLStick(Input::Stick::Left) || Input::GetInstance()->PushButton(Input::Button::DPAD_LEFT)) {
		if (++intervalCount_ >= moveInterval_) {
			if (stageNumber_ != 1) {
				stageNumber_--;
				modeRequest_ = Mode::Move;
			}
		}
	}

	if (Input::GetInstance()->ReleaseLStick(Input::Stick::Right) || Input::GetInstance()->ReleaseButton(Input::Button::DPAD_RIGHT)) {
		intervalCount_ = 0;
	}
	else if (Input::GetInstance()->ReleaseLStick(Input::Stick::Left) || Input::GetInstance()->ReleaseButton(Input::Button::DPAD_LEFT)) {
		intervalCount_ = 0;
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		if (stageNumber_ != kMaxStage_) {
			stageNumber_++;
			modeRequest_ = Mode::Move;
		}
	}
	else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		if (stageNumber_ != 1) {
			stageNumber_--;
			modeRequest_ = Mode::Move;
		}
	}

	if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		if (++intervalCount_ >= moveInterval_) {
			if (stageNumber_ != kMaxStage_) {
				stageNumber_++;
				modeRequest_ = Mode::Move;
			}
		}
	}
	else if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		if (++intervalCount_ >= moveInterval_) {
			if (stageNumber_ != 1) {
				stageNumber_--;
				modeRequest_ = Mode::Move;
			}
		}
	}

	if (Input::GetInstance()->ReleaseKey(DIK_RIGHT) || Input::GetInstance()->ReleaseKey(DIK_LEFT)) {
		intervalCount_ = 0;
	}

#endif // _DEBUG

	stageNumber_ = max(1, min(stageNumber_, kMaxStage_));

	const float step = 2.0f * std::numbers::pi_v<float> / (float)arrowFloating_.cycle_;

	arrowFloating_.param_ += step;

	arrowFloating_.param_ = std::fmod(arrowFloating_.param_, 2.0f * std::numbers::pi_v<float>);

	LArrowPos_.x += std::sinf(arrowFloating_.param_) * arrowFloating_.amplitude_;
	RArrowPos_.x += -std::sinf(arrowFloating_.param_) * arrowFloating_.amplitude_;

	uis_["LArrow"]->SetPosition(LArrowPos_);
	uis_["RArrow"]->SetPosition(RArrowPos_);

	stageNumberDraw_->AnimationUpdate();

}

void StageSelectScene::MoveInit() {

	param_ = 0.0f;

	moveStartPos_ = 640.0f * (preStageNum_ - 1);
	moveEndPos_ = 640.0f * (stageNumber_ - 1);

	uis_["RArrow"]->DrawOff();
	uis_["LArrow"]->DrawOff();

	intervalCount_ = 0;

}

void StageSelectScene::MoveUpdate() {

	float prePos = camera_.translation_.x;

	param_ += 0.06f;
	param_ = min(param_, 1.0f);

	float T = Easing::easeInOutQuart(param_);
	camera_.translation_.x = Lerp(T, moveStartPos_, moveEndPos_);
	
	float distance = camera_.translation_.x - prePos;
	player_[Blue]->position_.x += distance;
	player_[Orange]->position_.x += distance;
	uint32_t index = stageNumber_ - 1;
	uint32_t preIndex = preStageNum_ - 1;

	if (index != preIndex) {
		saunaRooms_[index]->SetScale(Lerp(T, 0.3f, 1.2f));
		saunaRooms_[preIndex]->SetScale(Lerp(T, 1.2f, 0.3f));
	}

	if (param_ >= 1.0f) {
		modeRequest_ = Mode::Root;
	}

}

void StageSelectScene::EnterInit() {
	
	animationNum_ = 0;
	animationTime_ = 0;
	player_[Blue]->SetTextureHandle(playerRunTex_[Blue]);
	player_[Orange]->SetTextureHandle(playerRunTex_[Orange]);
	uis_["RArrow"]->DrawOff();
	uis_["LArrow"]->DrawOff();

}

void StageSelectScene::EnterUpdate() {

	player_[Blue]->position_.x += 5.0f;
	player_[Orange]->position_.x += 5.0f;

	if (player_[Orange]->position_.x >= saunaRooms_[stageNumber_ - 1]->position_.x - 40.0f) {
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
