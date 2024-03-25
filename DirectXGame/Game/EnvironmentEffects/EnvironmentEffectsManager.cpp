#include "EnvironmentEffectsManager.h"

EnvironmentEffectsManager::EnvironmentEffectsManager() {
	prevScene.reset(new PostEffect());
	prevScene->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	nextScene.reset(new PostEffect());
	nextScene->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");
	dualSceneDrawer_.reset(new DualSceneDrawer());
	
	weightCircle_.reset(new WeightCircle);
	weightCircle_->Init(); dualSceneDrawer_->Init();
	isNowScene_ = false;
	isChangeComplete_ = false;
}

void EnvironmentEffectsManager::Update() {
	weightCircle_->Update();
	isChangeComplete_ = false;
	if (weightCircle_->GetIsEnd()) {
		isNowScene_ = !isNowScene_;
		weightCircle_->Reset();
		isChangeComplete_ = true;
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