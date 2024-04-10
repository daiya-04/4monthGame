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
	stage_->Initialize(stageNumber_);
	player_->SetBlocks(stage_->GetBlocks());

	camera_ = std::make_shared<Camera>();
	camera_->Init();
	camera_->translation_ = { 0.0f,0.0f,0.0f };
	stage_->SetCamera(camera_.get());

	scroll_ = std::make_unique<Scroll>();
	scroll_->Initialize();
	scroll_->SetTarget(player_->GetPositionPtr());
	scroll_->SetCamera(camera_.get());

	bgTexture_ = TextureManager::Load("backGround/backGround.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

}

void GameScene::Update(){
	DebugGUI();

	stage_->Update();

	player_->Update();

	scroll_->Update();

	camera_->UpdateMatrix();

}

void GameScene::DrawBackGround(){

	backGround_->Draw();

}

void GameScene::DrawObject(){

	stage_->Draw();

	player_->Draw(*camera_.get());

}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){

	

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


