#include "StageSelectScene.h"

#include "DirectXCommon.h"
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
	infoBaseTex_ = TextureManager::Load("UI/infoBase.png");
	gaugeBGTex_ = TextureManager::Load("UI/selectScoreBG.png");
	gaugeFreamTex_ = TextureManager::Load("UI/selectScoreFrame.png");
	gaugeTex_ = TextureManager::Load("UI/scoreGage.png");
	circleTex_ = TextureManager::Load("circle.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

	circle_.reset(Sprite::Create(circleTex_, {}));
	circle_->SetPosition(uis_["BButton"]->GetPosition());

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
	stageNumberDraw_->Init({ 638.0f,255.0f }, { 96.0f, 96.0f });
	stageNumberDraw_->SetStageNumber(stageNumber_);

	///UIの設定

	uis_["RArrow"]->SetSize({ 64.0f,64.0f });
	uis_["RArrow"]->SetTextureArea({ 64.0f,64.0f }, { 64.0f,64.0f });

	uis_["LArrow"]->SetSize({ 64.0f,64.0f });
	uis_["LArrow"]->SetTextureArea({}, { 64.0f,64.0f });

	uis_["BButton"]->SetScale(0.6f);

	infoBase_.reset(Sprite::Create(infoBaseTex_, { 640.0f,660.0f }));
	infoBase_->SetAnchorpoint({ 0.5f,1.0f });
	infoBase_->SetScale(0.7f);

	gaugeBG_.reset(Sprite::Create(gaugeBGTex_, { 640.0f,420.0f }));
	gaugeBG_->SetScale(0.7f);

	gaugeFream_.reset(Sprite::Create(gaugeFreamTex_, { 640.0f,420.0f }));
	gaugeFream_->SetScale(0.7f);

	gauge_.reset(Sprite::Create(gaugeTex_, {355.0f,420.0f}));
	gauge_->SetAnchorpoint({ 0.0f,0.5f });
	gauge_->SetSize({ 560.0f,96.0f });

	///

	steam_ = std::make_unique<Steam>();
	steam_->Init({ 640.0f,360.0f }, { 1280.0f,360.0f });
	steam_->SetEmitCount(6);

	score_.Init(scorePos_, { 64.0f,64.0f });
	score_.SetSpace(36.0f);

	rank_.Init(rankPos_, { 80.0f,80.0f });

	selectBGM_ = AudioManager::GetInstance()->Load("BGM/selectBGM.mp3");

	selectSE_ = AudioManager::GetInstance()->Load("SE/select_ok.mp3");
	moveSE_ = AudioManager::GetInstance()->Load("SE/select_move.mp3");
	cancelSE_ = AudioManager::GetInstance()->Load("SE/select_cancel.mp3");

	selectBGM_->Play();

	ButtonEffectInit();
	BoundingInit(uis_["ToTitle"]->GetPosition());
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

	ButtonEffectUpdate();
	BoundingUpdate(uis_["ToTitle"].get());

	steam_->SetCenter({ 640.0f + camera_.translation_.x,360.0f });
	steam_->Update();

	player_[Blue]->SetTextureArea({160.0f * animationNum_,160.0f}, {160.0f,160.0f});
	player_[Orange]->SetTextureArea({ 160.0f * animationNum_,160.0f }, { 160.0f,160.0f });

	camera_.UpdateViewMatrix();
}

void StageSelectScene::DrawBackGround() {

	backGround_->Draw();

	Particle::preDraw();
	steam_->Draw(camera_);

	Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());

	infoBase_->Draw();
	gaugeBG_->Draw();
	gauge_->Draw();
	gaugeFream_->Draw();

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
	circle_->Draw();
	for (const auto& ui : uis_) {
		ui.second->Draw();
	}

	if (mode_ != Mode::Move) {
		stageNumberDraw_->Draw();
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

	infoBase_->DrawOn();
	gaugeBG_->DrawOn();
	gauge_->DrawOn();
	gaugeFream_->DrawOn();

	ScoreGaugeSet();

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
				moveSE_->Play();
				modeRequest_ = Mode::Move;
			}
		}
	}
	else if (Input::GetInstance()->TiltLStick(Input::Stick::Left) || Input::GetInstance()->PushButton(Input::Button::DPAD_LEFT)) {
		if (++intervalCount_ >= moveInterval_) {
			if (stageNumber_ != 1) {
				stageNumber_--;
				moveSE_->Play();
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

	infoBase_->DrawOff();
	gaugeBG_->DrawOff();
	gauge_->DrawOff();
	gaugeFream_->DrawOff();

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

void StageSelectScene::ScoreGaugeSet() {

	if (ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) > 0) {

		//倍率設定
		float gageMag = float(score_.GetScore()) /
			(float(ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1)) * 1.15f);
		//1.0fを超えないようにする
		gageMag = std::clamp(gageMag, 0.0f, 1.0f);

		gauge_->SetSize({ 585.0f * gageMag,96.0f });
		gauge_->SetTextureArea({ 0.0f,0.0f },
			{ 380.0f * gageMag, 96.0f });

		//ゲージの色変化

		//S、黄金色
		if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1)) {
			gauge_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		}
		//A、赤色
		else if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) / 4 * 3) {
			gauge_->SetColor({ 1.0f,0.3f,0.3f,1.0f });
		}
		//B、黄色
		else if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) / 2) {
			gauge_->SetColor({ 1.0f,1.0f,0.3f,1.0f });
		}
		//C、緑色
		else if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) / 4) {
			gauge_->SetColor({ 0.3f,1.0f,0.3f,1.0f });
		}
		//D、水色
		else {
			gauge_->SetColor({ 0.3f,1.0f,1.0f,1.0f });
		}

	}
	else {
		gauge_->SetSize({ 422.0f,96.0f });
		gauge_->SetTextureArea({ 0.0f,0.0f },
			{ 422.0f, 96.0f });
		gauge_->SetColor({ 1.0f,1.0f,0.7f,1.0f });
	}

}

void StageSelectScene::ButtonEffectInit() {

	buttonEffect_.param_ = 0.0f;
	buttonEffect_.alpha_ = 1.0f;

}

void StageSelectScene::ButtonEffectUpdate() {

	buttonEffect_.param_ += 0.04f;

	buttonEffect_.param_ = std::fmod(buttonEffect_.param_, 1.0f);

	buttonEffect_.alpha_ = Lerp(buttonEffect_.param_, 1.0f, 0.0f);
	buttonEffect_.scale_ = Lerp(buttonEffect_.param_, 0.5f, 0.8f);

	circle_->SetColor({ 1.0f,1.0f,1.0f,buttonEffect_.alpha_ });
	circle_->SetScale(buttonEffect_.scale_);
}

void StageSelectScene::BoundingInit(const Vector2& axisPos) {

	workBounding_.axisPos_ = axisPos;
	workBounding_.velocity_.x = 0.5f;
	workBounding_.accel_ = {};
	workBounding_.addPos_ = {};

}

void StageSelectScene::BoundingUpdate(Sprite* sprite) {

	workBounding_.accel_.x += -0.001f;

	workBounding_.velocity_.x += workBounding_.accel_.x;

	workBounding_.addPos_ += workBounding_.velocity_;

	if (workBounding_.addPos_.x < 0) {
		BoundingInit(workBounding_.axisPos_);
	}

	sprite->SetPosition(workBounding_.axisPos_ + workBounding_.addPos_);

}

