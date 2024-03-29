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
	camera_->translation_ = { 0.0f,0.0f,0.0f };
	stage_->SetCamera(camera_.get());

	scroll_ = std::make_unique<Scroll>();
	scroll_->Initialize();
	scroll_->SetTarget(player_->GetPositionPtr());
	scroll_->SetCamera(camera_.get());

}

void GameScene::Update(){
	DebugGUI();

	stage_->Update();

	player_->Update();

	camera_->UpdateMatrix();

	scroll_->Update();

	for (auto& block : stage_->GetBlocks()) {

		//画面内のみ当たり判定チェック
		if (block->GetPosition().x >= camera_->translation_.x - Block::kBlockSize_ &&
			block->GetPosition().x <= camera_->translation_.x + 1280 + Block::kBlockSize_ &&
			block->GetPosition().y >= camera_->translation_.y - Block::kBlockSize_ &&
			block->GetPosition().y <= camera_->translation_.y + 720 + Block::kBlockSize_) {

			//当たり判定チェック
			if (IsCollision(block->GetCollision(), player_->GetCollision())) {

				Vector2 tmp{};

				//左上が当たっていた
				if (IsCollision(block->GetCollision(), player_->GetPosition(Player::kLeftTop))) {

					//プレイヤーがブロックより左側にいたなら右に押し戻し
					if (player_->GetPosition().x < block->GetPosition().x + Block::kBlockHalfSize_) {
						player_->SetPosition({ block->GetPosition().x + Block::kBlockHalfSize_ , player_->GetPosition().y });
					}

					//プレイヤーがブロックより上側にいたなら下に押し戻し
					if (player_->GetPosition().y < block->GetPosition().y + Block::kBlockHalfSize_) {
						player_->SetPosition({ player_->GetPosition().x,block->GetPosition().y + Block::kBlockHalfSize_ });
					}

				}
				//右上が当たっていた
				if (IsCollision(block->GetCollision(), player_->GetPosition(Player::kRightTop))) {

					//プレイヤーがブロックより右側にいたなら左に押し戻し
					if (player_->GetPosition().x > block->GetPosition().x - Block::kBlockHalfSize_) {
						player_->SetPosition({ block->GetPosition().x - Block::kBlockHalfSize_ , player_->GetPosition().y });
					}

					//プレイヤーがブロックより上側にいたなら下に押し戻し
					if (player_->GetPosition().y < block->GetPosition().y + Block::kBlockHalfSize_) {
						player_->SetPosition({ player_->GetPosition().x,block->GetPosition().y + Block::kBlockHalfSize_ });
					}

				}

				//左下が当たっていた
				if (IsCollision(block->GetCollision(), player_->GetPosition(Player::kLeftBottom))) {

					//プレイヤーがブロックより左側にいたなら右に押し戻し
					if (player_->GetPosition().x < block->GetPosition().x + Block::kBlockHalfSize_) {
						player_->SetPosition({ block->GetPosition().x + Block::kBlockHalfSize_ , player_->GetPosition().y });
					}

					//プレイヤーがブロックより下側にいたなら上に押し戻し
					if (player_->GetPosition().y > block->GetPosition().y - Block::kBlockHalfSize_) {
						player_->SetPosition({ player_->GetPosition().x,block->GetPosition().y - Block::kBlockHalfSize_ });
						player_->SetCanJump(true);
						player_->SetIsFly(false);
						player_->ResetVelocityY();
					}

				}

				//右下が当たっていた
				if (IsCollision(block->GetCollision(), player_->GetPosition(Player::kRightBottom))) {

					//プレイヤーがブロックより右側にいたなら左に押し戻し
					if (player_->GetPosition().x > block->GetPosition().x - Block::kBlockHalfSize_) {
						player_->SetPosition({ block->GetPosition().x - Block::kBlockHalfSize_ , player_->GetPosition().y });
					}

					//プレイヤーがブロックより下側にいたなら上に押し戻し
					if (player_->GetPosition().y > block->GetPosition().y - Block::kBlockHalfSize_) {
						player_->SetPosition({ player_->GetPosition().x,block->GetPosition().y - Block::kBlockHalfSize_ });
						player_->SetCanJump(true);
						player_->SetIsFly(false);
						player_->ResetVelocityY();
					}

				}

				/*player_->SetPosition(tmp);*/

			}

		}

	}

}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawObject(){

	stage_->Draw();

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
	ImGui::DragFloat3("translation", &camera_->translation_.x, 1.0f);
	ImGui::DragFloat3("rotation", &camera_->rotation_.x, 0.01f);
	ImGui::End();

	player_->Debug();

#endif // _DEBUG
}


