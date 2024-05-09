#include "GameScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>
#include "Game/Block/BlockTextureManager.h"

GameScene::~GameScene() {}

void GameScene::Init(){

	BlockTextureManager::GetInstance()->LoadAllBlockTexture();

	player_ = std::make_shared<Player>();
	player_->Initialize();

	stage_ = std::make_unique<Stage>();
	stage_->SetPlayer(player_.get());
	stage_->Initialize();
	stage_->Load(stageNumber_);
	player_->SetBlocks(stage_->GetBlocks());

	camera_ = std::make_shared<Camera>();
	camera_->Init();
	camera_->translation_ = { 0.0f,0.0f,0.0f };
	stage_->SetCamera(camera_.get());

	scroll_ = std::make_unique<Scroll>();
	scroll_->SetCamera(camera_.get());
	scroll_->Initialize();
	scroll_->SetTarget(player_->GetPositionPtr());
	

	bgTexture_ = TextureManager::Load("backGround/backGround.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

}

void GameScene::Reset() {

	player_->Initialize();
	stage_->Load(stageNumber_);
	camera_->Init();
	scroll_->Initialize();

}

void GameScene::Update(){
	DebugGUI();

	
#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_1)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_2)) {
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}
	if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}

#endif // _DEBUG

	if (player_->GetIsDead()) {

		if (Input::GetInstance()->TriggerButton(Input::Button::B)) {
			Reset();
		}

	}

	if (player_->GetIsBirdsEye()) {
		scroll_->SetTarget(player_->GetBirdsEyePositionPtr());
		camera_->ChangeDrawingRange({ 1920,1080.0f });
	}
	else {
		scroll_->SetTarget(player_->GetPositionPtr());
		camera_->ChangeDrawingRange({ 1280.0f,720.0f });
	}

	stage_->Update();

	player_->Update();

	scroll_->Update();

	camera_->UpdateMatrix();

}

void GameScene::DrawBackGround(){

	backGround_->Draw();

}

void GameScene::DrawObject(){

	player_->Draw(*camera_.get());

	stage_->Draw();

}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){

	player_->DrawUI();

	stage_->DrawUI();

}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &camera_->translation_.x, 1.0f);
	ImGui::DragFloat3("rotation", &camera_->rotation_.x, 0.01f);
	ImGui::End();

	player_->Debug();

#endif // _DEBUG
}


