#include "TutorialFlagManager.h"
#include "GameText/GameTextManager.h"

TutorialFlagManager* TutorialFlagManager::GetInstance() {
	static TutorialFlagManager instance;
	return &instance;
}

void TutorialFlagManager::Initialize() {

	for (int32_t i = 0; i < kMaxTutorial_; i++) {
		flags_[i] = false;
	}

	flags_[0] = true;

	tutorialNumber_ = 0;

}

void TutorialFlagManager::Update() {

	//フラグが立つまでチャージジャンプ制限
	if (!flags_[2]) {
		player_->RestrictChargeJump();
	}

	switch (tutorialNumber_)
	{
	case 0:

		//井戸の中に落ちたら会話開始
		if (player_->GetPosition().y > 300.0f && player_->GetCanJump()) {
			GameTextManager::GetInstance()->Tutorial(1);
			tutorialNumber_ = 1;
		}

		break;
	case 1:

		//常に止めておく
		magma_->freezeTime_ = 2;

		//赤い鉱石を取ったら会話開始
		if (player_->GetRockParameter().rocks_[Player::BringRocks::kRed] >= 1 &&
			player_->GetPosition().y > 1000.0f && player_->GetPosition().x < 480.0f && player_->GetCanJump()) {
			GameTextManager::GetInstance()->Tutorial(2);
			tutorialNumber_ = 2;
		}

		break;
	case 2:

		//一定の深さに達するまでマグマ停止
		if (player_->GetPosition().y < 1160.0f && !flags_[2]) {
			magma_->freezeTime_ = 2;
		}
		//特定ポジションでフラグセット
		else {
			//チャージジャンプ解禁
			flags_[2] = true;
		}

		//マグマが動き出した後に戻ったら会話開始
		if (player_->GetPosition().y > 487.0f && player_->GetPosition().y < 489.0f && player_->GetCanJump() 
			&& !player_->GetIsDead() && flags_[2]) {
			GameTextManager::GetInstance()->Tutorial(3);
			tutorialNumber_ = 3;
		}

		break;
	case 3:

		//続けて会話
		GameTextManager::GetInstance()->Tutorial(4);
		tutorialNumber_ = 4;

		break;
	case 4:

		//家に戻ったら会話開始
		if (player_->GetIsHome()) {
			GameTextManager::GetInstance()->Tutorial(5);
			tutorialNumber_ = 5;
		}

		break;
	case 5:

		//井戸の中に落ちたら会話開始
		if (player_->GetPosition().y > 300.0f && player_->GetCanJump()) {
			GameTextManager::GetInstance()->Tutorial(6);
			tutorialNumber_ = 6;
		}

		break;
	case 6:

		//家に戻ったら会話開始
		if (player_->GetIsHome()) {
			GameTextManager::GetInstance()->Tutorial(7);
			tutorialNumber_ = 7;
		}

		break;
	case 7:

		//黄金を取ったら会話開始
		if (player_->GetIsClear()) {
			GameTextManager::GetInstance()->Tutorial(8);
			tutorialNumber_ = 8;
		}

		break;
	case 8:



		break;
	case 9:



		break;
	default:
		break;
	}

}
