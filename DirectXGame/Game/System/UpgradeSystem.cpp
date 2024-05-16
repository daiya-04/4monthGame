#include "UpgradeSystem.h"
#include "TextureManager.h"
#include "Player/Player.h"

UpgradeSystem::UpgradeSystem()
{

	furnaceTexture_ = TextureManager::GetInstance()->Load("stageObject/furnace.png");

	furnace_.reset(Object2d::Create(furnaceTexture_, {Block::kBlockSize_ * 19.5f, Block::kBlockSize_ * 2.5f}));
	furnace_->SetSize({ Block::kBlockSize_ * 2.0f,Block::kBlockSize_ * 4.0f });

}

UpgradeSystem::~UpgradeSystem()
{
}

void UpgradeSystem::Initialize() {

	input_ = Input::GetInstance();

	isActive_ = false;

}

void UpgradeSystem::Update() {

	preIsActive_ = isActive_;

	if (isActive_ && preIsActive_) {

		//選択したものによって処理を切り替え
		if (isActiveUpgrade_) {



		}
		else {

			if (input_->TriggerLStick(Input::Stick::Up)) {

			}
			else if (input_->TriggerLStick(Input::Stick::Down)) {

			}

			if (input_->TriggerButton(Input::Button::A)) {

				switch (type_)
				{
				default:
				case UpgradeSystem::kSendRock:



					break;
				
				case UpgradeSystem::kUpgrade:



					break;
				
				case UpgradeSystem::kReturn:

					isActive_ = false;

					break;
				
				}

			}

		}

	}

}

void UpgradeSystem::SendRock() {



}

void UpgradeSystem::Upgrade() {



}

void UpgradeSystem::Draw(const Camera& camera) {

	furnace_->Draw(camera);

}
