#include "SnowManager.h"

#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "RandomEngine/RandomEngine.h"
#include "Stage/Stage.h"
void SnowManager::Init() {

	
	for (int i = 0; i < 2; i++) {
		internalEffectTextures_[i].reset(new SnowObstacle());
		internalEffectTextures_[i]->Init();
	}
	latestTextureNum_ = 0;
	std::unique_ptr<Object2d> originalSnow;
	originalSnow.reset(Object2d::Create(TextureManager::GetInstance()->Load("White.png"), {90.0f,2.0f}));
	originalSnow->SetSize({1296.0f+48.0f,48.0f});
	originalSnow->position_ = {Stage::kBasePosition.x,-48.0f};
	originalSnows_.push_back(std::move(originalSnow));
	originalSnow.reset(Object2d::Create(TextureManager::GetInstance()->Load("White.png"), { 90.0f,2.0f }));
	originalSnow->SetSize({ 9.0f*96.0f,48.0f });
	originalSnow->position_ = { 96.0f*4 ,-48.0f };
	originalSnows_.push_back(std::move(originalSnow));
	originalSnow.reset(Object2d::Create(TextureManager::GetInstance()->Load("White.png"), { 90.0f,2.0f }));
	originalSnow->SetSize({ 9.0f*96.0f,48.0f });
	originalSnow->position_ = { Stage::kBasePosition.x + 96.0f * 15 + 48.0f ,-48.0f };
	originalSnows_.push_back(std::move(originalSnow));
	snowPositionTexture_.reset(new PostEffect());
	snowPositionTexture_->Init(L"Resources/shaders/PostEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

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
	for (std::unique_ptr<Object2d> &originalSnow : originalSnows_) {
		originalSnow->Draw(camera);
	}
	Object2d::postDraw();
	snowPositionTexture_->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());

}

void SnowManager::Draw() {
	snowEffectDrawer_->Draw(DirectXCommon::GetInstance()->GetCommandList(),snowPositionTexture_->GetSrvHandleGPU(), internalEffectTextures_[latestTextureNum_]->GetSrvHandleGPU());
}