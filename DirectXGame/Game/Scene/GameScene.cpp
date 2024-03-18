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

	camera_ = std::make_shared<Camera>();
	camera_->Init();
	camera_->translation_ = { -100.0f,-100.0f,0.0f };

}

void GameScene::Update(){
	DebugGUI();

	stage_->Update();

	player_->Update();

	camera_->UpdateMatrix();

}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawObject(){

	stage_->Draw(*camera_.get());

	player_->Draw(*camera_.get());

}

void GameScene::DrawParticleModel(){



}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){

	

}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &camera_->translation_.x, 0.1f);
	ImGui::DragFloat3("rotation", &camera_->rotation_.x, 0.01f);
	ImGui::End();

#endif // _DEBUG
}


