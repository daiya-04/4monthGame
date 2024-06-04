#include "UpgradeSystem.h"
#include "TextureManager.h"
#include "Player/Player.h"
#include "DirectXCommon.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

UpgradeSystem::UpgradeSystem()
{

	furnaceTexture_ = TextureManager::GetInstance()->Load("stageObject/furnace.png");

	upgradeTexture_ = TextureManager::GetInstance()->Load("UI/powerUp.png");
	numberTexture_ = TextureManager::GetInstance()->Load("UI/number.png");
	rockUITextures_[0] = TextureManager::GetInstance()->Load("UI/rock.png");
	rockUITextures_[1] = TextureManager::GetInstance()->Load("UI/powerRock.png");
	rockUITextures_[2] = TextureManager::GetInstance()->Load("UI/digSpeedRock.png");
	rockUITextures_[3] = TextureManager::GetInstance()->Load("UI/speedRock.png");

	furnace_.reset(Object2d::Create(furnaceTexture_, {Block::kBlockSize_ * 19.5f, Block::kBlockSize_ * 2.5f}));
	furnace_->SetSize({ Block::kBlockSize_ * 2.0f,Block::kBlockSize_ * 4.0f });
	upgradeSprite_.reset(Sprite::Create(upgradeTexture_, { 640.0f,360.0f }));
	upgradeSprite_->SetSize({ 960.0f, 540.0f });
	upgradeSprite_->SetTextureArea({ 0.0f,0.0f }, { 960.0f,540.0f });

	//数字リセット
	for (int32_t x = 0; x < 4; x++) {

		float xPosition = 0.0f;
		float yPosition = 200.0f;

		if (x % 2 == 0) {
			xPosition = 480.0f - 128.0f;
		}
		else {
			xPosition = 480.0f + 128.0f;
		}

		if (x < 2) {
			yPosition = 64.0f + 200.0f;
		}
		else {
			yPosition = 164.0f + 200.0f;
		}

		for (int32_t i = 0; i < kMaxDigits_; i++) {

			numbers_[x][i].reset(Sprite::Create(numberTexture_, { xPosition + 48.0f * i , yPosition }));
			numbers_[x][i]->SetSize({ 64.0f,64.0f });
			numbers_[x][i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

		}

		//岩UIリセット
		rocksUI_[x].reset(Sprite::Create(rockUITextures_[x], { xPosition - 48.0f, yPosition }));
		rocksUI_[x]->SetSize({ 64.0f,64.0f });

	}

	for (int32_t i = 0; i < 2; i++) {
		levels_[i].reset(Sprite::Create(numberTexture_, { 540.0f + 64.0f * i , 450.0f}));
		levels_[i]->SetSize({ 96.0f,96.0f });
		levels_[i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });
	}


	upgradeCloseSE_ = AudioManager::GetInstance()->Load("SE/upgradeClose.mp3");
	upgradeSE_ = AudioManager::GetInstance()->Load("SE/powerUp.mp3");
	moveSE_ = AudioManager::GetInstance()->Load("SE/select_move.mp3");

}

UpgradeSystem::~UpgradeSystem()
{
}

void UpgradeSystem::Initialize(int32_t stageNum) {

	input_ = Input::GetInstance();

	LoadData(stageNum);

	for (int32_t i = 0; i < 2; i++) {
		speedLevel_[i] = 0;
		powerLevel_[i] = 0;
	}

	isActive_ = false;

}

void UpgradeSystem::Update() {

	preIsActive_ = isActive_;

	if (isActive_ && preIsActive_) {

		Upgrade();

		//数字の更新
		for (int32_t x = 0; x < 4; x++) {

			for (int32_t i = 0; i < kMaxDigits_; i++) {

				int32_t num = 0;

				int32_t divide = int32_t(std::pow(10, kMaxDigits_ - 1 - i));

				switch (upgradeType_)
				{
				default:
				case UpgradeSystem::kPower:
					num = powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][x] / divide;
					break;
				case UpgradeSystem::kDigSpeed:
					num = digSpeedUpgradeNeeds_[digSpeedLevel_[player_->GetCurrentCharacter()]][x] / divide;
					break;
				case UpgradeSystem::kSpeed:
					num = speedUpgradeNeeds_[speedLevel_[player_->GetCurrentCharacter()]][x] / divide;
					break;
				}

				numbers_[x][i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

			}

		}

		for (int32_t i = 0; i < 2; i++) {
			
			int32_t num = 0;

			int32_t divide = int32_t(std::pow(10, 2 - 1 - i));

			switch (upgradeType_)
			{
			default:
			case UpgradeSystem::kPower:
				
				num = (powerLevel_[player_->GetCurrentCharacter()] + 1) / divide;
				
				if (powerLevel_[player_->GetCurrentCharacter()] >= kMaxLevel_ - 1) {
					levels_[i]->SetColor({ 1.0f,1.0f,0.0f,1.0f });
				}
				else {
					levels_[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
				}
				
				break;
			case UpgradeSystem::kDigSpeed:
				
				num = (digSpeedLevel_[player_->GetCurrentCharacter()] + 1) / divide;

				if (digSpeedLevel_[player_->GetCurrentCharacter()] >= kMaxLevel_ - 1) {
					levels_[i]->SetColor({ 1.0f,1.0f,0.0f,1.0f });
				}
				else {
					levels_[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
				}

				break;
			case UpgradeSystem::kSpeed:
				num = (speedLevel_[player_->GetCurrentCharacter()] + 1) / divide;

				if (speedLevel_[player_->GetCurrentCharacter()] >= kMaxLevel_ - 1) {
					levels_[i]->SetColor({ 1.0f,1.0f,0.0f,1.0f });
				}
				else {
					levels_[i]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
				}

				break;
			}

			levels_[i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

		}

	}
	//強化画面を開いていない時はゲームの更新をする
	else {



	}

}

void UpgradeSystem::SendRock() {

	isActiveUpgrade_ = false;
	isActive_ = false;

}

void UpgradeSystem::Upgrade() {

	if (input_->TriggerLStick(Input::Stick::Up)) {

		switch (upgradeType_)
		{
		case UpgradeSystem::kSpeed:
			upgradeType_ = kDigSpeed;
			break;
		case UpgradeSystem::kDigSpeed:
			upgradeType_ = kPower;
			break;
		default:
			break;
		}

		moveSE_->Play();

	}
	else if (input_->TriggerLStick(Input::Stick::Down)) {

		switch (upgradeType_)
		{
		case UpgradeSystem::kPower:
			upgradeType_ = kDigSpeed;
			break;
		case UpgradeSystem::kDigSpeed:
			upgradeType_ = kSpeed;
			break;
		default:
			break;
		}

		moveSE_->Play();

	}

	//Bボタンで戻れる
	if (input_->TriggerButton(Input::Button::B)) {

		isActiveUpgrade_ = false;
		isActive_ = false;
		upgradeType_ = kPower;
		upgradeCloseSE_->Play();

	}

	if (input_->TriggerButton(Input::Button::A)) {

		switch (upgradeType_)
		{
		default:
		case UpgradeSystem::kSpeed:

			CheckCanUpgrade(kSpeed);

			break;
		case UpgradeSystem::kPower:

			CheckCanUpgrade(kPower);

			break;
		case UpgradeSystem::kDigSpeed:

			CheckCanUpgrade(kDigSpeed);

			break;
		}

	}

	switch (upgradeType_)
	{
	default:
	case UpgradeSystem::kSpeed:

		upgradeSprite_->SetTextureArea({ 960.0f * 2.0f,0.0f }, { 960.0f,540.0f });

		break;

	case UpgradeSystem::kPower:

		upgradeSprite_->SetTextureArea({ 0.0f,0.0f }, { 960.0f,540.0f });

		break;

	case UpgradeSystem::kDigSpeed:

		upgradeSprite_->SetTextureArea({ 960.0f,0.0f }, { 960.0f,540.0f });

		break;

	}

}

void UpgradeSystem::CheckCanUpgrade(SelectType type) {

	switch (type)
	{
	default:
	case UpgradeSystem::kSpeed:

		if (speedLevel_[player_->GetCurrentCharacter()] < kMaxLevel_ - 1) {

			//条件を満たしていたらブロックを消費して強化
			if (player_->GetRockParameter().rocks_[Player::BringRocks::kRock] >=
				speedUpgradeNeeds_[speedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRock] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kBlue] >=
				speedUpgradeNeeds_[speedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kBlue] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kGreen] >=
				speedUpgradeNeeds_[speedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kGreen] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kRed] >=
				speedUpgradeNeeds_[speedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRed]) {

				for (int32_t i = 0; i < Player::BringRocks::kMaxType; i++) {

					player_->GetRockParameter().rocks_[i] -= speedUpgradeNeeds_[speedLevel_[player_->GetCurrentCharacter()]][i];

				}

				player_->UpgradeSpeed(speedUpgradeValue_);
				speedLevel_[player_->GetCurrentCharacter()]++;
				upgradeSE_->Play();

			}
			else {

			}

		}
		else {

		}

		break;
	case UpgradeSystem::kPower:

		if (powerLevel_[player_->GetCurrentCharacter()] < kMaxLevel_ - 1) {

			//条件を満たしていたらブロックを消費して強化
			if (player_->GetRockParameter().rocks_[Player::BringRocks::kRock] >=
				powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRock] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kBlue] >=
				powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kBlue] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kGreen] >=
				powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kGreen] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kRed] >=
				powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRed]) {

				for (int32_t i = 0; i < Player::BringRocks::kMaxType; i++) {

					player_->GetRockParameter().rocks_[i] -= powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][i];

				}

				player_->UpgradePower(powerUpgradeValue_);
				powerLevel_[player_->GetCurrentCharacter()]++;
				upgradeSE_->Play();

			}
			else {

			}

		}
		else {

		}

		break;
	case UpgradeSystem::kDigSpeed:

		if (digSpeedLevel_[player_->GetCurrentCharacter()] < kMaxLevel_ - 1) {

			//条件を満たしていたらブロックを消費して強化
			if (player_->GetRockParameter().rocks_[Player::BringRocks::kRock] >=
				digSpeedUpgradeNeeds_[digSpeedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRock] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kBlue] >=
				digSpeedUpgradeNeeds_[digSpeedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kBlue] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kGreen] >=
				digSpeedUpgradeNeeds_[digSpeedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kGreen] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kRed] >=
				digSpeedUpgradeNeeds_[digSpeedLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRed]) {

				for (int32_t i = 0; i < Player::BringRocks::kMaxType; i++) {

					player_->GetRockParameter().rocks_[i] -= digSpeedUpgradeNeeds_[digSpeedLevel_[player_->GetCurrentCharacter()]][i];

				}

				player_->UpgradeDigSpeed(digSpeedUpgradeValue_);
				digSpeedLevel_[player_->GetCurrentCharacter()]++;
				upgradeSE_->Play();

			}
			else {

			}

		}
		else {

		}

		break;
	}

}

void UpgradeSystem::LoadData(int32_t stageNum) {

	std::string name = "./Resources/Data/Upgrade/up1";
	/*name += std::to_string(stageNum);*/
	name += ".csv";

	std::ifstream file(name);

	if (file.fail()) {
		return;
	}

	//一行分の文字を格納する文字列
	std::string line;

	for (int32_t i = 0; i < 10; i++) {

		//一行取得
		std::getline(file, line);

		//カンマ区切りで読み込む用の文字列
		std::istringstream iss(line);

		for (int32_t k = 0; k < 8; k++) {

			//格納する数字
			std::string sNum;
			//数字を格納
			std::getline(iss, sNum, ',');

			if (k < 4) {
				speedUpgradeNeeds_[i][k] = std::stoi(sNum);
				speedUpgradeNeeds_[i][k] = std::clamp(speedUpgradeNeeds_[i][k], 0, 999);
			}
			else {
				powerUpgradeNeeds_[i][k - 4] = std::stoi(sNum);
				digSpeedUpgradeNeeds_[i][k - 4] = std::stoi(sNum);
				powerUpgradeNeeds_[i][k - 4] = std::clamp(powerUpgradeNeeds_[i][k - 4], 0, 999);
				digSpeedUpgradeNeeds_[i][k - 4] = std::clamp(digSpeedUpgradeNeeds_[i][k - 4], 0, 999);
			}

		}

	}

}

void UpgradeSystem::Draw(const Camera& camera) {

	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());

	furnace_->Draw(camera);	

}

void UpgradeSystem::DrawUI() {

	if (isActive_) {

		upgradeSprite_->Draw();

		//数字描画
		for (int32_t x = 0; x < 4; x++) {


			switch (upgradeType_)
			{
			default:
			case UpgradeSystem::kPower:
				
				if (powerLevel_[player_->GetCurrentCharacter()] < kMaxLevel_ - 1) {

					for (int32_t i = 0; i < kMaxDigits_; i++) {
						numbers_[x][i]->Draw();
					}

					rocksUI_[x]->Draw();

				}

				break;
			case UpgradeSystem::kDigSpeed:
				
				if (digSpeedLevel_[player_->GetCurrentCharacter()] < kMaxLevel_ - 1) {

					for (int32_t i = 0; i < kMaxDigits_; i++) {
						numbers_[x][i]->Draw();
					}

					rocksUI_[x]->Draw();

				}

				break;
			case UpgradeSystem::kSpeed:
				
				if (speedLevel_[player_->GetCurrentCharacter()] < kMaxLevel_ - 1) {

					for (int32_t i = 0; i < kMaxDigits_; i++) {
						numbers_[x][i]->Draw();
					}

					rocksUI_[x]->Draw();

				}

				break;
			}

		}

		for (int32_t i = 0; i < 2; i++) {
			levels_[i]->Draw();
		}

	}

}
