#include "SnowManager.h"

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "RandomEngine/RandomEngine.h"
void SnowManager::Init() {

	
	for (int i = 0; i < 2; i++) {
		internalEffectTextures_[i].reset(new SnowObstacle());
		internalEffectTextures_[i]->Init();
	}
	latestTextureNum_ = 0;
	
	originalSnow.reset(Object2d::Create(TextureManager::GetInstance()->Load("White.png"), {10.0f,2.0f}));
	originalSnow->SetSize({400.0f,100.0f});
	originalSnow->position_ = {640.0f,360.0f};
	snowPositionTexture_.reset(new PostEffect());
	snowPositionTexture_->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	snowEffectDrawer_.reset(new SnowEffectDrawer);
	snowEffectDrawer_->Init();
	isClearEffect_ = true;
}

void SnowManager::PreDrawUpdateEffect() {
	if (isClearEffect_) {
		for (int i = 0; i < 2; i++) {
			internalEffectTextures_[i]->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
			internalEffectTextures_[i]->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
		}
		isClearEffect_ = false;
	}

	latestTextureNum_ = bool(!latestTextureNum_);
	internalEffectTextures_[latestTextureNum_]->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
	internalEffectTextures_[!latestTextureNum_]->SetCamelaSlide(cameraSlide_);
	internalEffectTextures_[!latestTextureNum_]->Draw(DirectXCommon::GetInstance()->GetCommandList());
	//新しい位置の書き込み処理
	
}

void SnowManager::PostDrawUpdateEffect(const Camera& camera) {
	

	internalEffectTextures_[latestTextureNum_]->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());

	snowPositionTexture_->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	originalSnow->Draw(camera);
	Object2d::postDraw();
	snowPositionTexture_->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());

}

void SnowManager::Draw() {
	snowEffectDrawer_->Draw(DirectXCommon::GetInstance()->GetCommandList(),snowPositionTexture_->GetSrvHandleGPU(), internalEffectTextures_[latestTextureNum_]->GetSrvHandleGPU());
}