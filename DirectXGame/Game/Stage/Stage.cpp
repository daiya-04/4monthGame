#include "Stage.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Game/Player/Player.h"
#include "TextureManager.h"
#include <cmath>
#include "Block/BlockTextureManager.h"
#include "DirectXCommon.h"
#include "ImGuiManager.h"
#include "System/TutorialFlagManager.h"
#include "GameText/GameTextManager.h"

std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight> Stage::map_;

Stage::Stage()
{

	numTex_ = TextureManager::GetInstance()->Load("UI/number.png");
	borderBlueTex_ = TextureManager::GetInstance()->Load("stageObject/flagBlue.png");
	borderOrangeTex_ = TextureManager::GetInstance()->Load("stageObject/flagOrange.png");
	saunaRoomTex_ = TextureManager::GetInstance()->Load("stageObject/saunaRoom.png");
	purposeTex_ = TextureManager::GetInstance()->Load("UI/mokuteki.png");
	upTex_ = TextureManager::GetInstance()->Load("UI/up.png");
	ropeTex_ = TextureManager::GetInstance()->Load("stageObject/rope.png");
	tutorialFirstTex_ = TextureManager::GetInstance()->Load("UI/tutorialUI1.png");
	tutorialSecondTex_ = TextureManager::GetInstance()->Load("UI/tutorialUI2.png");
	tutorialThirdTex_ = TextureManager::GetInstance()->Load("UI/tutorialUI3.png");
	wellBlueTex_ = TextureManager::GetInstance()->Load("stageObject/wellBlue.png");
	wellOrangeTex_ = TextureManager::GetInstance()->Load("stageObject/wellOrange.png");
	UI_A_Tex_ = TextureManager::GetInstance()->Load("AButton.png");
	UI_X_Tex_ = TextureManager::GetInstance()->Load("UI/XButton.png");
	telescopeTex_ = TextureManager::GetInstance()->Load("stageObject/telescope.png");

	for (int32_t i = 0; i < kMaxNumbers_; i++) {

		numbers_[i].reset(Sprite::Create(numTex_, {1050.0f + 48.0f * i, 660.0f}));
		numbers_[i]->SetSize({ 64.0f,64.0f });
		numbers_[i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

	}

	tutorialFirst_.reset(Object2d::Create(tutorialFirstTex_, Vector2{3.5f * Block::kBlockSize_, 3.5f * Block::kBlockSize_ }));
	tutorialSecond_.reset(Object2d::Create(tutorialSecondTex_, Vector2{ 3.5f * Block::kBlockSize_, 3.5f * Block::kBlockSize_ }));
	tutorialThird_.reset(Object2d::Create(tutorialThirdTex_, Vector2{ 14.0f * Block::kBlockSize_, 13.5f * Block::kBlockSize_ }));
	tutorialThird_->SetSize({ 2.0f * Block::kBlockSize_, 4.0f * Block::kBlockSize_ });

	saunaRoom_.reset(Object2d::Create(saunaRoomTex_, kBasePosition - Vector2{0.0f, 18.0f}));
	telescope_.reset(Object2d::Create(telescopeTex_, kBasePosition - Vector2{ -200.0f, -12.0f }));
	telescope_->SetSize({ 64.0f,64.0f });
	UI_A_.reset(Object2d::Create(UI_A_Tex_, kBasePosition - Vector2{ 0.0f, 150.0f }));
	UI_A_->SetSize({ 64.0f,64.0f });
	UI_X_.reset(Object2d::Create(UI_X_Tex_, kBasePosition - Vector2{ -200.0f, 50.0f }));
	UI_X_->SetSize({ 64.0f,64.0f });
	ropes_[0].reset(Object2d::Create(ropeTex_, Vector2{ 10.5f * Block::kBlockSize_, 1.5f * Block::kBlockSize_ }));
	ropes_[0]->SetSize({ 2.0f * Block::kBlockSize_,8.0f * Block::kBlockSize_ });
	ropes_[1].reset(Object2d::Create(ropeTex_, Vector2{ 28.5f * Block::kBlockSize_, 1.5f * Block::kBlockSize_ }));
	ropes_[1]->SetSize({ 2.0f * Block::kBlockSize_,8.0f * Block::kBlockSize_ });
	wells_[0].reset(Object2d::Create(wellOrangeTex_, Vector2{ 10.5f * Block::kBlockSize_, 0.0f * Block::kBlockSize_ }));
	wells_[0]->SetSize({ 2.0f * Block::kBlockSize_,1.0f * Block::kBlockSize_ });
	wells_[1].reset(Object2d::Create(wellBlueTex_, Vector2{ 28.5f * Block::kBlockSize_, 0.0f * Block::kBlockSize_ }));
	wells_[1]->SetSize({ 2.0f * Block::kBlockSize_,1.0f * Block::kBlockSize_ });

	purposeSprite_.reset(Sprite::Create(purposeTex_, { 640.0f,200.0f }));

	borders_[0].reset(Object2d::Create(borderOrangeTex_, kBorderLeft));
	borders_[0]->SetAnchorpoint({ 0.5f,1.0f });
	borders_[0]->SetSize({ 64.0f,192.0f });
	borders_[0]->SetTextureArea({ 0.0f,0.0f }, { 32.0f,96.0f });
	borders_[1].reset(Object2d::Create(borderBlueTex_, kBorderRight));
	borders_[1]->SetAnchorpoint({ 0.5f,1.0f });
	borders_[1]->SetSize({ 64.0f,192.0f });
	borders_[1]->SetTextureArea({ 0.0f,0.0f }, { 32.0f,96.0f });

	magma_ = std::make_unique<Magma>();

	returnPosition_[0] = { 10.5f * Block::kBlockSize_, 5.0f * Block::kBlockSize_ };
	returnPosition_[1] = { 28.5f * Block::kBlockSize_, 5.0f * Block::kBlockSize_ };

	returnUI_.reset(Object2d::Create(upTex_, returnPosition_[0] + Vector2{ 0.0f,-100.0f }));

	returnArea_[0].max = { returnPosition_[0].x + Block::kBlockSize_, returnPosition_[0].y + Block::kBlockHalfSize_ };
	returnArea_[0].min = { returnPosition_[0].x - Block::kBlockSize_, returnPosition_[0].y - Block::kBlockHalfSize_ };

	returnArea_[1].max = { returnPosition_[1].x + Block::kBlockSize_, returnPosition_[1].y + Block::kBlockHalfSize_ };
	returnArea_[1].min = { returnPosition_[1].x - Block::kBlockSize_, returnPosition_[1].y - Block::kBlockHalfSize_ };

	upgradePosition_ = { 19.5f * Block::kBlockSize_, 4.0f * Block::kBlockSize_ };

	upgradeArea_.max = { upgradePosition_.x + Block::kBlockHalfSize_ * 3.0f, upgradePosition_.y + Block::kBlockHalfSize_ };
	upgradeArea_.min = { upgradePosition_.x - Block::kBlockHalfSize_ * 3.0f, upgradePosition_.y - Block::kBlockHalfSize_ };

	upgradeSystem_ = std::make_unique<UpgradeSystem>();
	upgradeSystem_->SetGoalCount(&rockCount_);

	environmentEffectsManager_ = EnvironmentEffectsManager::GetInstance();

	CreateEntity();

	meltSE_ = AudioManager::GetInstance()->Load("SE/melt.mp3");
	upgradeOpenSE_ = AudioManager::GetInstance()->Load("SE/upgradeOpen.mp3");

}

Stage::~Stage()
{
}

void Stage::Initialize(uint32_t stageNumber) {

	EnemyManager::GetInstance()->Initialize();
	EnemyManager::GetInstance()->SetMap(&map_);
	EnemyManager::GetInstance()->SetPlayer(player_);

	currentStageNumber_ = stageNumber;

	for (int32_t y = 0; y < kMaxStageHeight; y++) {

		for (int32_t x = 0; x < kMaxStageWidth; x++) {

			//一旦初期化。耐久力などの設定はLoadで行う
			blockPositions_[y][x] = 0;
			map_[y][x]->SetPlayer(player_);
			map_[y][x]->SetMagma(magma_.get());
			map_[y][x]->SetDurability(float(y / 5) + 3.0f);

			//上下左右のブロックのポインタ設定
			Block* pLeft = nullptr;
			Block* pRight = nullptr;
			Block* pUp = nullptr;
			Block* pDown = nullptr;

			if (y - 1 >= 0) {
				pUp = map_[y - 1][x].get();
			}
			
			if (y + 1 < kMaxStageHeight) {
				pDown = map_[y + 1][x].get();
			}

			if (x - 1 >= 0) {
				pLeft = map_[y][x - 1].get();
			}

			if (x + 1 < kMaxStageWidth) {
				pRight = map_[y][x + 1].get();
			}

			map_[y][x]->SetPointer(pLeft, pRight, pUp, pDown);

		}

	}

	upgradeSystem_->Initialize(stageNumber);
	upgradeSystem_->SetPlayer(player_);

	isStart_ = false;
	isClear_ = false;
	isRespawn_ = true;
	isBreakFlagBlocks_ = false;
	canUpgrade_ = false;
	rockCount_ = 0;
	magma_->Initialize();
	magma_->SetPlayer(player_);

	Load(stageNumber);

}

void Stage::Update() {

#ifdef _DEBUG

	

#endif // _DEBUG

	//プレイヤーがクリアフラグを持った状態でサウナ室に帰ったらクリアフラグ立てる
	if (player_->GetIsClear() && player_->GetIsHome()) {

		isClear_ = true;

	}


	if (!isClear_) {

		//チュートリアルステージ用更新
		if (currentStageNumber_ == 1) {

			if (TutorialFlagManager::GetInstance()->GetFlags(2) && !isBreakFlagBlocks_) {
				BreakFlagBlock();
			}

			//画像切り替え用の変数カウント
			if (++tutorialSwitchCount_ > maxSwitchCount_) {
				tutorialSwitchCount_ = 0;
			}

		}

		//ブロックの更新
		for (uint32_t y = 0; y < kMaxStageHeight; y++) {

			for (uint32_t x = 0; x < kMaxStageWidth; x++) {
				blockPositions_[y][x] = map_[y][x]->GetType();
				SetUV(map_[y][x].get());
				map_[y][x]->Update();
			}

		}

		//敵更新
		EnemyManager::GetInstance()->Update();

		//家に戻ったら再生成
		if (player_->GetIsHome() && !isRespawn_) {
			RespawnBlock(Block::kDownMagma);
			RespawnBlock(Block::kCollapseBlock);

			//極寒なら氷生成
			if (!environmentEffectsManager_->GetIsNowScene()) {
				CreateIceBlock();
			}

			isRespawn_ = true;
		}
		else if (player_->GetIsDead()) {
			RespawnBlock(Block::kGoldBlock);
		}
		else if (player_->GetIsMine()) {
			isRespawn_ = false;
		}

		upgradeSystem_->Update();

		//強化中は上昇しない
		if (!upgradeSystem_->GetIsActive()) {

			magma_->Update();

		}

		//当たり判定更新
		CheckCollision();

		//数字更新
		for (int32_t i = 0; i < kMaxNumbers_; i++) {

			int32_t num = 0;

			int32_t divide = int32_t(std::pow(10, kMaxNumbers_ - 1 - i));

			num = rockCount_ / divide;

			//割る数の方が大きい且つnumが0の状態で一桁でない時、数字を表示しない
			if (num == 0 && rockCount_ < divide && divide != 1) {
				isActiveNumber_[i] = false;
			}
			else {
				isActiveNumber_[i] = true;
			}

			numbers_[i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

		}

		BlockTextureManager::GetInstance()->UpdateParticle(*camera_);

	}
	//クリアした時
	else {

	}

	UpdateAnimation();
	
}

void Stage::UpdateAnimation() {

	//画像を動かす処理
	if (++flagAnimationTime_ >= flagChangeFrame_) {

		flagAnimationTime_ = 0;

		//設定した最大数に達したらリセット
		if (++currentFlagAnimationNum_ >= maxFlagAnimationNum_) {
			currentFlagAnimationNum_ = 0;
		}

	}

	borders_[0]->SetTextureArea({ 32.0f * currentFlagAnimationNum_,0.0f }, { 32.0f,96.0f });
	borders_[1]->SetTextureArea({ 32.0f * currentFlagAnimationNum_,0.0f }, { 32.0f,96.0f });


}

void Stage::CheckCollision() {

	if (IsCollision(returnArea_[0], player_->GetCollision())) {

		//ステージ1の時だけ特殊処理
		if (currentStageNumber_ == 1) {

			if (TutorialFlagManager::GetInstance()->GetFlags(2)) {

				player_->MoveLift();
				canReturn_ = true;

				returnUI_->position_ = returnPosition_[0] + Vector2{ 0.0f,-100.0f };

			}
			else {
				canReturn_ = false;
			}

		}
		else {
			player_->MoveLift();
			canReturn_ = true;

			returnUI_->position_ = returnPosition_[0] + Vector2{ 0.0f,-100.0f };
		}

	}
	else if (IsCollision(returnArea_[1], player_->GetCollision())) {

		if (currentStageNumber_ == 1) {

			if (TutorialFlagManager::GetInstance()->GetFlags(2)) {

				player_->MoveLift();
				canReturn_ = true;

				returnUI_->position_ = returnPosition_[1] + Vector2{ 0.0f,-100.0f };

			}
			else {
				canReturn_ = false;
			}

		}
		else {
			player_->MoveLift();
			canReturn_ = true;

			returnUI_->position_ = returnPosition_[1] + Vector2{ 0.0f,-100.0f };
		}

	}
	else {
		canReturn_ = false;
	}

	//特定エリアでボタンを押したら強化画面に移行
	if (IsCollision(upgradeArea_, player_->GetCollision()) &&
		player_->GetCanJump() && !upgradeSystem_->GetPreIsActive()) {

		canUpgrade_ = true;

		UI_A_->position_ = player_->GetPosition() - Vector2{ 0.0f,100.0f };

		if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
			upgradeSystem_->SetIsActive(true);
			upgradeOpenSE_->Play();
		}

	}
	else {
		canUpgrade_ = false;
		UI_A_->position_ = kBasePosition - Vector2{ 0.0f, 150.0f };
	}

}

void Stage::Draw() {

	//リスト初期化
	BlockTextureManager::GetInstance()->ClearObject();

	//ブロックの描画
	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {

			if (map_[y][x]->GetPosition().x >= camera_->translation_.x - Block::kBlockSize_ &&
				map_[y][x]->GetPosition().x <= camera_->translation_.x + camera_->GetDrawingRange().x + Block::kBlockSize_ &&
				map_[y][x]->GetPosition().y >= camera_->translation_.y - Block::kBlockSize_ &&
				map_[y][x]->GetPosition().y <= camera_->translation_.y + camera_->GetDrawingRange().y + Block::kBlockSize_) {
				map_[y][x]->Draw(*camera_);
			}
			
		}

	}
	/*
	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	BlockTextureManager::GetInstance()->DrawAll(*camera_);
	BlockTextureManager::GetInstance()->DrawParticle(*camera_);
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (uint32_t i = 0; i < 2; i++) {
		borders_[i]->Draw(*camera_);
		returnObjects_[i]->Draw(*camera_);
	}

	if (canReturn_) {
		returnUI_->Draw(*camera_);
	}

	upgradeSystem_->Draw(*camera_);

	magma_->Draw(*camera_);
	*/
}

void Stage::DrawHeatBefore() {

	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (uint32_t i = 0; i < 2; i++) {
		ropes_[i]->Draw(*camera_);
	}
	BlockTextureManager::GetInstance()->DrawChargeCompleteEffect(*camera_);
}

void Stage::DrawHeatAfter() {

	
	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	BlockTextureManager::GetInstance()->DrawHeat(*camera_);
	BlockTextureManager::GetInstance()->DrawParticle(*camera_);
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (uint32_t i = 0; i < 2; i++) {
		borders_[i]->Draw(*camera_);
		wells_[i]->Draw(*camera_);
	}

	if (currentStageNumber_ == 1) {

		if (tutorialSwitchCount_ < maxSwitchCount_ / 2) {
			tutorialFirst_->Draw(*camera_);
		}
		else {
			tutorialSecond_->Draw(*camera_);
		}

		tutorialThird_->Draw(*camera_);

	}

	if (canReturn_) {
		returnUI_->Draw(*camera_);
	}

	EnemyManager::GetInstance()->Draw(*camera_);

	upgradeSystem_->Draw(*camera_);

	magma_->Draw(*camera_);

	saunaRoom_->Draw(*camera_);
	telescope_->Draw(*camera_);

	if ((player_->GetIsHome() && GameTextManager::GetInstance()->GetIsEnd() && isStart_) ||
		canUpgrade_) {
		UI_A_->Draw(*camera_);
		UI_X_->Draw(*camera_);
	}

}

void Stage::DrawColdBefore() {

	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (uint32_t i = 0; i < 2; i++) {
		ropes_[i]->Draw(*camera_);
	}
	BlockTextureManager::GetInstance()->DrawChargeCompleteEffect(*camera_);
}

void Stage::DrawColdAfter() {


	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	BlockTextureManager::GetInstance()->DrawCold(*camera_);
	BlockTextureManager::GetInstance()->DrawParticle(*camera_);
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (uint32_t i = 0; i < 2; i++) {
		borders_[i]->Draw(*camera_);
		wells_[i]->Draw(*camera_);
	}

	if (currentStageNumber_ == 1) {

		if (tutorialSwitchCount_ < maxSwitchCount_ / 2) {
			tutorialFirst_->Draw(*camera_);
		}
		else {
			tutorialSecond_->Draw(*camera_);
		}

		tutorialThird_->Draw(*camera_);

	}

	if (canReturn_) {
		returnUI_->Draw(*camera_);
	}

	EnemyManager::GetInstance()->Draw(*camera_);

	upgradeSystem_->Draw(*camera_);

	magma_->Draw(*camera_);

	saunaRoom_->Draw(*camera_);
	telescope_->Draw(*camera_);

	if ((player_->GetIsHome() && GameTextManager::GetInstance()->GetIsEnd() && isStart_) ||
		canUpgrade_) {
		UI_A_->Draw(*camera_);
		UI_X_->Draw(*camera_);
	}

}

void Stage::DrawUI() {

	/*for (uint32_t i = 0; i < kMaxNumbers_; i++) {

		if (isActiveNumber_[i]) {
			numbers_[i]->Draw();
		}

	}*/

	upgradeSystem_->DrawUI();

	if (player_->GetIsHome()) {
		/*purposeSprite_->Draw();*/
	}

	/*if (isClear_) {
		clearSprite_->Draw();
	}*/

}

void Stage::SwitchBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {
			
			if (map_[y][x]->GetType() == Block::BlockType::kSnow) {
				map_[y][x]->ChangeType(Block::BlockType::kMagma);
			}
			else if (map_[y][x]->GetType() == Block::BlockType::kMagma) {
				map_[y][x]->ChangeType(Block::BlockType::kSnow);
			}

			blockPositions_[y][x] = map_[y][x]->GetType();

		}

	}

}

void Stage::CreateIceBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		
		for (uint32_t x = 0; x < kMaxStageWidth; x++) {
			
			//一部スキップ
			if (y < 5) {

				continue;

			}
			else if (y == 5) {

				if ((x >= 8 && x <= 13) || (x >= 26 && x <= 31)) {
					continue;
				}
				else if (x <= 5 || (x >= 16 && x <= 23) || x >= 34) {
					continue;
				}

			}
			else if (y == 6) {

				if ((x >= 9 && x <= 12) || (x >= 27 && x <= 30)) {
					continue;
				}

			}

			bool isContinue = false;

			//再生成されるブロックは書き換えない
			for (int32_t i = 0; i < respawnBlocks_.size(); i++) {

				if (y == respawnBlocks_[i][1] && x == respawnBlocks_[i][0]) {
					isContinue = true;
				}

			}

			if (isContinue) {
				continue;
			}

			//ブロックが無い且つプレイヤーと当たらない部分に氷ブロックを生成
			if (map_[y][x]->GetIsBreak() && !IsCollision(map_[y][x]->GetCollision(), player_->GetCollision())) {

				//敵と被ったら復活させない
				for (auto& enemy : EnemyManager::GetInstance()->GetEnemies()) {

					if (IsCollision(map_[y][x]->GetCollision(), enemy->GetCollision())) {
						isContinue = true;
						break;
					}

				}

				if (isContinue) {
					continue;
				}

				map_[y][x]->ChangeType(Block::BlockType::kIceBlock);
				map_[y][x]->SetColor();
				map_[y][x]->Repair();

				/*blockPositions_[y][x] = BaseBlock::BlockType::kIceBlock;*/

			}

		}

	}

}

void Stage::BreakIceBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {

			//氷ブロックを破壊する
			if (map_[y][x]->GetType() == Block::BlockType::kIceBlock) {

				map_[y][x]->MeltIce();

			}

		}

	}

	meltSE_->Play();

}

void Stage::BreakAllBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {

			//壊せないブロック以外を破壊する
			if (Block::CheckCanBreak(map_[y][x]->GetType())) {

				map_[y][x]->Break(999);

			}

		}

	}

}

void Stage::BreakFlagBlock() {

	if (currentStageNumber_ == 1) {
		map_[2][14]->ChangeType(Block::kNone);
		map_[3][14]->ChangeType(Block::kNone);
		isBreakFlagBlocks_ = true;
	}

}

void Stage::RespawnBlock(Block::BlockType type) {

	//再生成可能なブロックの中から対応した要素を再生成させる
	for (int32_t i = 0; i < respawnBlocks_.size(); i++) {

		if (map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->GetDefaultType() == type) {

			//スキップフラグ
			bool isContinue = false;

			//敵と被ったら復活させない
			for (auto& enemy : EnemyManager::GetInstance()->GetEnemies()) {

				if (IsCollision(map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->GetCollision(), enemy->GetCollision())) {
					isContinue = true;
					break;
				}

			}

			int32_t startY = int32_t(map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->GetPosition().y) / Block::kBlockSize_ - 1;
			startY = std::clamp(startY, 0, kMaxStageHeight - 1);
			int32_t endY = int32_t(map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->GetPosition().y) / Block::kBlockSize_ + 1;
			endY = std::clamp(endY, 0, kMaxStageHeight - 1);
			int32_t startX = int32_t(map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->GetPosition().x) / Block::kBlockSize_ - 1;
			startX = std::clamp(startX, 0, kMaxStageWidth - 1);
			int32_t endX = int32_t(map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->GetPosition().x) / Block::kBlockSize_ + 1;
			endX = std::clamp(endX, 0, kMaxStageWidth - 1);

			for (int32_t y = startY; y <= endY; y++) {

				for (int32_t x = startX; x <= endX; x++) {

					//自分と同じブロックは判定しない
					if (y == respawnBlocks_[i][1] && x == respawnBlocks_[i][0]) {
						continue;
					}



				}

			}
			if (isContinue) {
				continue;
			}

			map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->ChangeType(type);
			map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->Reset();
			//高さに応じて耐久値を調整
			if (type == Block::kGoldBlock) {
				map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->SetDurability(30.0f);
			}
			else if (type == Block::kDownMagma) {
				map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->SetDurability(5.0f);
			}
			else {
				map_[respawnBlocks_[i][1]][respawnBlocks_[i][0]]->SetDurability(float(respawnBlocks_[i][1] / 5) + 3.0f);
			}

		}

	}

}

void Stage::CreateEntity() {

	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {

			//実体がない場合、生成する
			if (!map_[y][x]) {

				//ブロックの実体を生成、初期化
				map_[y][x].reset(new Block({ x * float(Block::kBlockSize_), y * float(Block::kBlockSize_) }, Block::BlockType::kNone));
				map_[y][x]->Initialize({ x * float(Block::kBlockSize_), y * float(Block::kBlockSize_) }, Block::BlockType::kNone);
				map_[y][x]->SetPlayer(player_);
				map_[y][x]->SetMagma(magma_.get());
				map_[y][x]->SetBlockPosition(x, y);

			}

		}

	}

}

void Stage::Load(uint32_t stageNumber) {

	//パーツの残り数をリセット
	rockCount_ = 0;
	//再生成できるブロックの要素リセット
	respawnBlocks_.clear();

	std::string fileName = "./Resources/Maps/stage";

	fileName += std::to_string(stageNumber);

	fileName += ".csv";

	std::ifstream newFile(fileName);

	//ファイル読み込みが出来なかったら処理を止める
	if (newFile.fail()) {
		MessageBox(nullptr, L"ファイルを読み込めませんでした。", L"Stage - Load", 0);
		return;
	}

	//保険
	assert(!newFile.fail());

	//一行分の文字を格納する文字列
	std::string line;

	//ブロックを生成、初期化
	for (uint32_t y = 0; y < kMaxStageHeight; y++) {
		
		//一行取得
		std::getline(newFile, line);

		//カンマ区切りで読み込む用の文字列
		std::istringstream iss(line);

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {
			//格納する数字
			std::string sNum;
			//数字を格納
			std::getline(iss, sNum, ',');


			//番号からTypeを選ぶ
			uint32_t num = std::stoi(sNum, nullptr, 16);
			Block::BlockType type;
			//範囲外のものを選ばないように防ぐ
			if (num < Block::BlockType::kMaxBlock) {
				type = static_cast<Block::BlockType>(num);
			}
			else {
				num = 0;
				type = static_cast<Block::BlockType>(num);
			}

			//空気ブロックでなければブロックを生成
			//ブロックのパラメータ変更
			map_[y][x]->ChangeType(type);
			map_[y][x]->Reset();
			//高さに応じて耐久値を調整
			if (type == Block::kGoldBlock) {
				map_[y][x]->SetDurability(30.0f);
			}
			else if (type == Block::kDownMagma) {
				map_[y][x]->SetDurability(5.0f);
			}
			else if (type == Block::kNeedleBlock) {
				EnemyManager::GetInstance()->AddEnemy(map_[y][x]->GetPosition(), BaseEnemy::Type::kNeedle);
				map_[y][x]->ChangeType(Block::kNone);
			}
			else {
				map_[y][x]->SetDurability(float(y / 5) + 3.0f);
			}

			//タイプが再生成可能なものなら配列に追加
			if (type == Block::kDownMagma || type == Block::kGoldBlock || type == Block::kCollapseBlock) {
				//書き換え場所を保存
				std::array<uint32_t, 2> nums = { x,y };
				respawnBlocks_.push_back(nums);
			}

			blockPositions_[y][x] = num;

		}

	}

	//チュートリアルステージだけ特殊ブロック配置
	if (stageNumber == 1) {

		map_[2][14]->ChangeType(Block::kFlagBlock);
		map_[2][14]->Reset();
		map_[3][14]->ChangeType(Block::kFlagBlock);
		map_[3][14]->Reset();

	}

	//ブロックの更新
	for (uint32_t y = 0; y < kMaxStageHeight; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth; x++) {
			blockPositions_[y][x] = map_[y][x]->GetType();
			SetUV(map_[y][x].get());
			map_[y][x]->Update();
		}

	}

}

void Stage::SetUV(Block* block) {

	int32_t px = block->GetBlockPositionX();
	int32_t py = block->GetBlockPositionY();

	//周囲八マスの判定を取るための変数
	int32_t left = px - 1;
	int32_t right = px + 1;
	int32_t top = py - 1;
	int32_t bottom = py + 1;
	
	//周囲八マスと現在のブロックの数字
	int32_t centerNum = blockPositions_[py][px];
	int32_t leftNum = 0;
	int32_t rightNum = 0;
	int32_t topNum = 0;
	int32_t bottomNum = 0;
	int32_t topLeftNum = 0;
	int32_t topRightNum = 0;
	int32_t bottomLeftNum = 0;
	int32_t bottomRightNum = 0;

	//範囲外に出るかどうかで代入する値を変更
	if (left < 0 || left >= kMaxStageWidth) {

		leftNum = 0;
		topLeftNum = 0;
		bottomLeftNum = 0;


	}
	else {

		leftNum = blockPositions_[py][left];

	}

	if (right < 0 || right >= kMaxStageWidth) {

		rightNum = 0;
		topRightNum = 0;
		bottomRightNum = 0;

	}
	else {

		rightNum = blockPositions_[py][right];

	}

	if (top < 0 || top >= kMaxStageHeight) {

		topNum = 0;
		topLeftNum = 0;
		topRightNum = 0;

	}
	else {

		topNum = blockPositions_[top][px];

		if (left >= 0 && left < kMaxStageWidth) {
			topLeftNum = blockPositions_[top][left];
		}

		if (right >= 0 && right < kMaxStageWidth) {
			topRightNum = blockPositions_[top][right];
		}

	}

	if (bottom < 0 || bottom >= kMaxStageHeight) {

		bottomNum = 0;
		bottomLeftNum = 0;
		bottomRightNum = 0;

	}
	else {

		bottomNum = blockPositions_[bottom][px];

		if (left >= 0 && left < kMaxStageWidth) {
			bottomLeftNum = blockPositions_[bottom][left];
		}

		if (right >= 0 && right < kMaxStageWidth) {
			bottomRightNum = blockPositions_[bottom][right];
		}

	}

	//周囲八マスに同一ブロックがある場合
	if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(5, 0);
	}

	//左上以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(5, 4);
	}

	//右上以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(6, 4);
	}

	//左下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(4, 4);
	}

	//右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(7, 4);
	}

	//左上、左下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(0, 4);
	}

	//左上、右上以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(1, 4);
	}

	//左上、右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(6, 3);
	}

	//右上、左下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(5, 3);
	}

	//右上、右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(2, 4);
	}

	//左下、右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(3, 4);
	}

	//左上のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum != bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(3, 3);
	}

	//右上のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(4, 3);
	}

	//左下のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(2, 3);
	}

	//右下のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum != topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(1, 3);
	}

	//左以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topRightNum && centerNum == bottomRightNum) {
			block->SetUVPosition(1, 0);
	}

	//右以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == bottomLeftNum) {
			block->SetUVPosition(3, 0);
	}

	//上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
			block->SetUVPosition(2, 0);
	}

	//下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum) {
			block->SetUVPosition(4, 0);
	}

	//左と上以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomRightNum) {
			block->SetUVPosition(0, 1);
	}

	//右と上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomLeftNum) {
			block->SetUVPosition(1, 1);
	}

	//左と下以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topRightNum) {
			block->SetUVPosition(3, 1);
	}

	//右と下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topLeftNum) {
			block->SetUVPosition(2, 1);
	}

	//左と右上以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topRightNum && centerNum == bottomRightNum) {
			block->SetUVPosition(0, 6);
	}

	//左と右下以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topRightNum && centerNum != bottomRightNum) {
			block->SetUVPosition(1, 6);
	}

	//右と左上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == bottomLeftNum) {
			block->SetUVPosition(4, 6);
	}

	//右と左下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != bottomLeftNum) {
			block->SetUVPosition(5, 6);
	}

	//上と左下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
			block->SetUVPosition(2, 6);
	}

	//上と右下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
			block->SetUVPosition(3, 6);
	}

	//下と左上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum) {
			block->SetUVPosition(6, 6);
	}

	//下と右上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum) {
			block->SetUVPosition(7, 6);
	}

	//周囲四マスにブロックがある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(0, 3);
	}

	//周囲四マスに同一ブロックが無い場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(0, 0);
	}
	//右に1つだけある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(0, 2);
	}
	//左に1つだけある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(2, 2);
	}
	//上に1つだけある場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(3, 2);
	}
	//下に一つだけある場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(1, 2);
	}
	//右と下にある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(4, 5);
	}
	//左と下にある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(5, 5);
	}
	//右と上にある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(7, 5);
	}
	//左と上にある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(6, 5);
	}
	//左右にある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(5, 1);
	}
	//上下にある場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(4, 1);
	}
	//右以外にある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(2, 5);
	}
	//左以外にある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(0, 5);
	}
	//上以外にある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(1, 5);
	}
	//下以外にある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(3, 5);
	}

}

void Stage::ChangeSnow2Magma() {
	for (uint32_t y = 0; y < kMaxStageHeight; y++) {
		for (uint32_t x = 0; x < kMaxStageWidth; x++) {
			if (map_[y][x]->GetType() == Block::BlockType::kSnow) {
				map_[y][x]->ChangeType(Block::BlockType::kMagma);
			}
			blockPositions_[y][x] = map_[y][x]->GetType();
		}
	}
}

void Stage::ChangeMagma2Snow() {
	for (uint32_t y = 0; y < kMaxStageHeight; y++) {
		for (uint32_t x = 0; x < kMaxStageWidth; x++) {
			if (map_[y][x]->GetType() == Block::BlockType::kMagma) {
				map_[y][x]->ChangeType(Block::BlockType::kSnow);
			}
			blockPositions_[y][x] = map_[y][x]->GetType();
		}
	}
}
