#include "EnvironmentEffectsManager.h"
#include "Input.h"
EnvironmentEffectsManager::EnvironmentEffectsManager() {
	prevScene.reset(new PostEffect());
	prevScene->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	nextScene.reset(new PostEffect());
	nextScene->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");
	dualSceneDrawer_.reset(new DualSceneDrawer());
	
	weightCircle_.reset(new WeightCircle);
	weightCircle_->Init(); dualSceneDrawer_->Init();
	isNowScene_ = true;
	isChangeComplete_ = false;
	isPlayDownMagmaEffect_ = false;
}

void EnvironmentEffectsManager::Update() {
#ifdef _DEBUG
	/*if (Input::GetInstance()->TriggerKey(DIK_3)) {
		weightCircle_->Start();
	}*/
#endif // _DEBUG

	weightCircle_->Update();
	isChangeComplete_ = false;
	if (weightCircle_->GetIsEnd()) {
		isNowScene_ = !isNowScene_;
		weightCircle_->Reset();
		isChangeComplete_ = true;
	}

	if (isPlayDownMagmaEffect_) {
		weightCircle_->SetPosition(magmaCenter_ - Vector2{ camera_->translation_.x + camera_->center_.x - 640.0f,camera_->translation_.y + camera_->center_.y - 360.0f });
		if (isChangeComplete_) {
			isCompletefirstChange_ = true;
		}
		if (isCompletefirstChange_) {
			downMagmaEffectInterval_--;
		}
		if (downMagmaEffectInterval_<=0) {
			ChangeSceneMode();
			isPlayDownMagmaEffect_ = false;
		}
	}
}

void EnvironmentEffectsManager::Draw(ID3D12GraphicsCommandList* cmdList) {
	if (weightCircle_->GetIsStart()) {
		//遷移中の二重描画
		dualSceneDrawer_->Draw(cmdList, prevScene->GetSrvHandleGPU(), nextScene->GetSrvHandleGPU(), weightCircle_->GetHandle());
	}
	else {
		//通常時の描画
		prevScene->Draw(cmdList);
	}
}

void EnvironmentEffectsManager::ChangeSceneMode() {
	weightCircle_->Start();
}