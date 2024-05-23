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

	uint32_t texture = 0;
	texture = TextureManager::Load("blocks/defaultRocks.png");

	sendRocks_.reset(Object2dInstancing::Create(texture, { 0.0f,0.0f }, 128));
	sendRocks_->SetSize({ 32.0f,32.0f });
	sendRocks_->SetColor({ 0.6f,0.3f,0.1f,1.0f });

	furnaceTexture_ = TextureManager::GetInstance()->Load("stageObject/furnace.png");

	backTexture_ = TextureManager::GetInstance()->Load("UI/back.png");
	sendRockTexture_ = TextureManager::GetInstance()->Load("UI/sendRock.png");
	upgradeTexture_ = TextureManager::GetInstance()->Load("UI/upgrade.png");
	sendIntervalTexture_ = TextureManager::GetInstance()->Load("UI/sendInterval.png");
	powerTexture_ = TextureManager::GetInstance()->Load("UI/power.png");
	numberTexture_ = TextureManager::GetInstance()->Load("UI/number.png");
	rockUITextures_[0] = TextureManager::GetInstance()->Load("UI/rock.png");
	rockUITextures_[1] = TextureManager::GetInstance()->Load("UI/speedRock.png");
	rockUITextures_[2] = TextureManager::GetInstance()->Load("UI/digSpeedRock.png");
	rockUITextures_[3] = TextureManager::GetInstance()->Load("UI/powerRock.png");

	furnace_.reset(Object2d::Create(furnaceTexture_, {Block::kBlockSize_ * 19.5f, Block::kBlockSize_ * 2.5f}));
	furnace_->SetSize({ Block::kBlockSize_ * 2.0f,Block::kBlockSize_ * 4.0f });
	backSprite_.reset(Sprite::Create(backTexture_, UIBottomPosition_));
	sendRockSprite_.reset(Sprite::Create(sendRockTexture_, UITopPosition_));
	powerSprite_.reset(Sprite::Create(upgradeTexture_, UIMiddlePosition_));

	//数字リセット
	for (int32_t y = 0; y < 2; y++) {

		for (int32_t x = 0; x < 4; x++) {

			float xPosition = 0.0f;
			float yPosition = 0.0f;

			if (x % 2 == 0) {
				xPosition = 900.0f;
			}
			else {
				xPosition = 1000.0f;
			}

			if (x < 2) {
				yPosition = 50.0f + 200 * (y + 1);
			}
			else {
				yPosition = 100.0f + 200 * (y + 1);
			}

			for (int32_t i = 0; i < 2; i++) {

				numbers_[y][x][i].reset(Sprite::Create(numberTexture_, { xPosition + 24.0f * i , yPosition }));
				numbers_[y][x][i]->SetSize({ 32.0f,32.0f });
				numbers_[y][x][i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

			}

			//岩UIリセット
			rocksUI_[y][x].reset(Sprite::Create(rockUITextures_[x], { xPosition - 32.0f, yPosition }));
			rocksUI_[y][x]->SetSize({ 32.0f,32.0f });

		}

	}
	
}

UpgradeSystem::~UpgradeSystem()
{
}

void UpgradeSystem::Initialize(int32_t stageNum) {

	input_ = Input::GetInstance();

	LoadData(stageNum);

	sendRockLevel_ = 0;

	for (int32_t i = 0; i < 2; i++) {
		powerLevel_[i] = 0;
	}

	sendRockCount_ = 0;
	sendInterval_ = 150;
	sendCoolTime_ = 0;

	isActive_ = false;

}

void UpgradeSystem::Update() {

	preIsActive_ = isActive_;

	if (isActive_ && preIsActive_) {

		//選択したものによって処理を切り替え
		if (isActiveUpgrade_) {

			switch (type_)
			{
			default:
			case UpgradeSystem::kSendRock:

				SendRock();

				break;

			case UpgradeSystem::kUpgrade:

				Upgrade();

				break;

			}

			sendRockSprite_->SetTextureHandle(sendIntervalTexture_);
			powerSprite_->SetTextureHandle(powerTexture_);

		}
		else {

			if (input_->TriggerLStick(Input::Stick::Up)) {

				switch (type_)
				{
				case UpgradeSystem::kUpgrade:
					type_ = kSendRock;
					break;
				case UpgradeSystem::kReturn:
					type_ = kUpgrade;
					break;
				default:
					break;
				}

			}
			else if (input_->TriggerLStick(Input::Stick::Down)) {

				switch (type_)
				{
				case UpgradeSystem::kSendRock:
					type_ = kUpgrade;
					break;
				case UpgradeSystem::kUpgrade:
					type_ = kReturn;
					break;
				default:
					break;
				}

			}

			if (input_->TriggerButton(Input::Button::A)) {

				switch (type_)
				{
				default:
				case UpgradeSystem::kSendRock:

					isActiveUpgrade_ = true;

					break;
				
				case UpgradeSystem::kUpgrade:

					isActiveUpgrade_ = true;

					break;
				
				case UpgradeSystem::kReturn:

					isActive_ = false;

					break;
				
				}

			}

			//UIの動き付け
			switch (type_)
			{
			default:
			case UpgradeSystem::kSendRock:

				backSprite_->SetPosition({ 1000.0f,600.0f });
				sendRockSprite_->SetPosition({ 950.0f,200.0f });
				powerSprite_->SetPosition({ 1000.0f,400.0f });

				break;

			case UpgradeSystem::kUpgrade:

				backSprite_->SetPosition({ 1000.0f,600.0f });
				sendRockSprite_->SetPosition({ 1000.0f,200.0f });
				powerSprite_->SetPosition({ 950.0f,400.0f });

				break;

			case UpgradeSystem::kReturn:

				backSprite_->SetPosition({ 950.0f,600.0f });
				sendRockSprite_->SetPosition({ 1000.0f,200.0f });
				powerSprite_->SetPosition({ 1000.0f,400.0f });

				break;

			}

			sendRockSprite_->SetTextureHandle(sendRockTexture_);
			powerSprite_->SetTextureHandle(upgradeTexture_);

		}

		//数字の更新
		for (int32_t y = 0; y < 2; y++) {

			for (int32_t x = 0; x < 4; x++) {

				for (int32_t i = 0; i < 2; i++) {

					int32_t num = 0;

					int32_t divide = int32_t(std::pow(10, 2 - 1 - i));

					//岩送りの数字
					if (y == 0) {

						num = sendRockUpgradeNeeds_[sendRockLevel_][Player::BringRocks::kRock] / divide;

						numbers_[y][x][i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

					}
					////採掘力の数字
					else {

						num = powerUpgradeNeeds_[powerLevel_[player_->GetCurrentCharacter()]][Player::BringRocks::kRock] / divide;

						numbers_[y][x][i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

					}

				}

			}

		}

	}
	//強化画面を開いていない時はゲームの更新をする
	else {

		sendRocks_->ClearUseCount();

		if (sendRockCount_ > 0) {

			if (--sendCoolTime_ <= 0) {

				positions_.push_back(furnace_->position_);

				sendCoolTime_ = sendInterval_;
				sendRockCount_--;

			}

		}
		else {

			if (sendCoolTime_ > 0) {
				sendCoolTime_--;
			}

		}

		int32_t popCount = 0;

		for (int32_t i = 0; i < positions_.size(); i++) {

			positions_[i].y -= sendSpeed_;
			sendRocks_->AppendObject(positions_[i], { 0.0f,0.0f }, { 32.0f,32.0f });

			//一定ラインを超えたら削除カウント追加
			if (positions_[i].y <= Block::kBlockSize_) {
				popCount++;
			}

		}

		for (int32_t i = 0; i < popCount; i++) {

			//先頭のポジション削除、ゴールカウント増加
			positions_.erase(positions_.begin());
			(*goalCount_)++;

		}



	}

}

void UpgradeSystem::SendRock() {

	player_->SendRocks(sendRockCount_);

	isActiveUpgrade_ = false;
	isActive_ = false;

}

void UpgradeSystem::Upgrade() {

	if (input_->TriggerLStick(Input::Stick::Up)) {

		switch (upgradeType_)
		{
		case UpgradeSystem::kUpgrade:
			upgradeType_ = kSendRock;
			break;
		case UpgradeSystem::kReturn:
			upgradeType_ = kUpgrade;
			break;
		default:
			break;
		}

	}
	else if (input_->TriggerLStick(Input::Stick::Down)) {

		switch (upgradeType_)
		{
		case UpgradeSystem::kSendRock:
			upgradeType_ = kUpgrade;
			break;
		case UpgradeSystem::kUpgrade:
			upgradeType_ = kReturn;
			break;
		default:
			break;
		}

	}

	if (input_->TriggerButton(Input::Button::A)) {

		switch (upgradeType_)
		{
		default:
		case UpgradeSystem::kSendRock:

			CheckCanUpgrade(kSendRock);

			break;
		case UpgradeSystem::kUpgrade:

			CheckCanUpgrade(kUpgrade);

			break;
		case UpgradeSystem::kReturn:

			isActiveUpgrade_ = false;

			break;
		}

	}

	switch (upgradeType_)
	{
	default:
	case UpgradeSystem::kSendRock:

		backSprite_->SetPosition({ 1000.0f,600.0f });
		sendRockSprite_->SetPosition({ 950.0f,200.0f });
		powerSprite_->SetPosition({ 1000.0f,400.0f });

		break;

	case UpgradeSystem::kUpgrade:

		backSprite_->SetPosition({ 1000.0f,600.0f });
		sendRockSprite_->SetPosition({ 1000.0f,200.0f });
		powerSprite_->SetPosition({ 950.0f,400.0f });

		break;

	case UpgradeSystem::kReturn:

		backSprite_->SetPosition({ 950.0f,600.0f });
		sendRockSprite_->SetPosition({ 1000.0f,200.0f });
		powerSprite_->SetPosition({ 1000.0f,400.0f });

		break;

	}

}

void UpgradeSystem::CheckCanUpgrade(SelectType type) {

	switch (type)
	{
	default:
	case UpgradeSystem::kSendRock:

		if (sendRockLevel_ < kMaxLevel_) {

			//条件を満たしていたらブロックを消費して強化
			if (player_->GetRockParameter().rocks_[Player::BringRocks::kRock] >=
				sendRockUpgradeNeeds_[sendRockLevel_][Player::BringRocks::kRock] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kBlue] >=
				sendRockUpgradeNeeds_[sendRockLevel_][Player::BringRocks::kBlue] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kGreen] >=
				sendRockUpgradeNeeds_[sendRockLevel_][Player::BringRocks::kGreen] &&
				player_->GetRockParameter().rocks_[Player::BringRocks::kRed] >=
				sendRockUpgradeNeeds_[sendRockLevel_][Player::BringRocks::kRed]) {

				for (int32_t i = 0; i < Player::BringRocks::kMaxType; i++) {

					player_->GetRockParameter().rocks_[i] -= sendRockUpgradeNeeds_[sendRockLevel_][i];

				}

				sendInterval_ -= sendRockUpgradeValue_;
				sendRockLevel_++;

			}
			else {

			}

		}
		else {

		}

		break;
	case UpgradeSystem::kUpgrade:

		if (powerLevel_[player_->GetCurrentCharacter()] < kMaxLevel_) {

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

	std::string name = "./Resources/Data/Upgrade/up";
	name += std::to_string(stageNum);
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
				sendRockUpgradeNeeds_[i][k] = std::stoi(sNum);
			}
			else {
				powerUpgradeNeeds_[i][k - 4] = std::stoi(sNum);
			}

		}

	}

}

void UpgradeSystem::Draw(const Camera& camera) {

	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());

	furnace_->Draw(camera);

	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());

	sendRocks_->Draw(camera);

	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	

}

void UpgradeSystem::DrawUI() {

	if (isActive_) {

		sendRockSprite_->Draw();

		powerSprite_->Draw();

		backSprite_->Draw();

		if (isActiveUpgrade_) {

			//数字描画
			for (int32_t y = 0; y < 2; y++) {

				for (int32_t x = 0; x < 4; x++) {

					for (int32_t i = 0; i < 2; i++) {
						numbers_[y][x][i]->Draw();
					}

					rocksUI_[y][x]->Draw();

				}

			}

		}

	}

}
