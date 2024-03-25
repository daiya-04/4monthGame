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
}

void EnvironmentEffectsManager::Update() {
	weightCircle_->Update();
	if (weightCircle_->GetIsEnd()) {
		isNowScene_ = !isNowScene_;
		weightCircle_->Reset();
	}
}

void EnvironmentEffectsManager::Draw(ID3D12GraphicsCommandList* cmdList) {
	if (weightCircle_->GetIsStart()) {
		dualSceneDrawer_->Draw(cmdList, prevScene->GetSrvHandleGPU(), nextScene->GetSrvHandleGPU(), weightCircle_->GetHandle());
	}
	else {
		prevScene->Draw(cmdList);
	}
}