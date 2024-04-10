#include "WaterDropManager.h"
#include "DirectXCommon.h"
#include "WinApp.h"
void WaterDropManager::Init() {

	WaterDropPipeline::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), WinApp::kClientWidth, WinApp::kClientHeight);

	waterDrop_.reset(new WaterDrop());
	waterDrop_->Init();

	waterDropSceneDrawer_.reset(new WaterDropSceneDrawer);
	waterDropSceneDrawer_->Init();
}

void WaterDropManager::Update() {
	waterDrop_->Update();
}

void WaterDropManager::DrawEffectUpdate(PostEffect* sorceTexture) {
	waterDrop_->DrawInternal(sorceTexture);
	waterDrop_->DrawUpdateEffect();
}

void WaterDropManager::DrawScene(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE prevSceneHandle) {
	waterDropSceneDrawer_->Draw(cmdList, prevSceneHandle, waterDrop_->GetHandle());
}