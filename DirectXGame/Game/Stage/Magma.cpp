#include "Magma.h"
#include "Player/Player.h"
#include "Stage.h"
#include "ImGuiManager.h"
#include "Easing.h"

Magma::Magma()
{

	magmaTex_ = TextureManager::GetInstance()->Load("stageObject/magmaLine.png");
	magma_.reset(Object2d::Create(magmaTex_, { Stage::kBasePosition.x,magmaUnderLine_ }));
	magma_->SetColor({ 1.0f,1.0f,1.0f,0.3f });
	magma_->SetAnchorpoint({ 0.5f,1.0f });
	magma_->SetSize({ float(Block::kBlockSize_ * kMaxStageWidth), 64.0f });
	magma_->SetTextureArea({ 0.0f,0.0f }, { float(Block::kBlockSize_ * kMaxStageWidth), 32.0f });

}

Magma::~Magma()
{
}

void Magma::Initialize() {

	baseMagmaLine_ = maxMagmaLine_;
	currentMagmaLine_ = baseMagmaLine_;
	magmaTexBaseX_ = 0.0f;
	easingT_ = 0.0f;

}

void Magma::Update() {

#ifdef _DEBUG

	ImGui::Begin("magma");
	ImGui::DragFloat("line", &baseMagmaLine_, 1.0f);
	ImGui::DragFloat("speed", &magmaSpeed_, 0.1f);
	ImGui::Text("freezeTime : %d", freezeTime_);
	ImGui::End();

#endif // _DEBUG

	//採掘中にマグマライン上昇
	/*if (player_->GetIsMine()) {

	}*/

	//硬直時間中は動かない
	if (freezeTime_ > 0) {

		freezeTime_--;

	}
	else if (baseMagmaLine_ > magmaLimit_) {

		//プレイヤーが黄金を取得しているとスピードを上げるようにする
		if (player_->GetIsClear()) {
			baseMagmaLine_ -= magmaSpeedFaster_;
		}
		else {
			baseMagmaLine_ -= magmaSpeed_;
		}

		//ラインが限界値を超えたら限界値に調整
		if (baseMagmaLine_ < magmaLimit_) {
			baseMagmaLine_ = magmaLimit_;
		}

	}

	//サウナ室に戻った時にリセット
	/*else if (baseMagmaLine_ < maxMagmaLine_) {
		ResetMagma();
	}*/

	//マグマに当たったらスタートに戻る
	if (player_->GetPosition(Player::kLeftBottom).y >= currentMagmaLine_) {
		player_->Restart();
	}
	
	//採掘中と帰還中でストーカー速度を変更
	if (player_->GetIsMine()) {

		if (easingT_ < 1.0f) {
			
			easingT_ += 0.05f;
			
			if (easingT_ > 1.0f) {
				easingT_ = 1.0f;
			}

		}

		float t = Easing::easeOutBack(easingT_);

		currentMagmaLine_ = Lerp(t, currentMagmaLine_, baseMagmaLine_);

		/*currentMagmaLine_ += (baseMagmaLine_ - currentMagmaLine_) * 0.05f;*/

	}
	else {

		if (easingT_ < 1.0f) {

			easingT_ += 0.05f;

			if (easingT_ > 1.0f) {
				easingT_ = 1.0f;
			}

		}

		float t = Easing::easeOutBack(easingT_);

		currentMagmaLine_ = Lerp(t, currentMagmaLine_, baseMagmaLine_);

		/*currentMagmaLine_ += (baseMagmaLine_ - currentMagmaLine_) * 0.05f;*/

	}

	//テクスチャの動きを付ける
	magmaTexBaseX_++;
	if (magmaTexBaseX_ > 256.0f) {
		magmaTexBaseX_ = 0.0f;
	}

	//マグマ更新
	magma_->SetSize({ float(Block::kBlockSize_ * kMaxStageWidth), magmaUnderLine_ - currentMagmaLine_ });
	magma_->SetTextureArea({ magmaTexBaseX_,0.0f }, { float(Block::kBlockSize_ * kMaxStageWidth), magmaUnderLine_ - currentMagmaLine_ });

}

void Magma::ResetMagma() {

	baseMagmaLine_ = maxMagmaLine_;

}

void Magma::Draw(const Camera& camera) {

	magma_->Draw(camera);

}
