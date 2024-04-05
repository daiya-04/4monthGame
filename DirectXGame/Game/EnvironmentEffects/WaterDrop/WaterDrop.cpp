#include "WaterDrop.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "WaterDropPipeline.h"
void WaterDrop::Init() {

	sorceTexture_.reset(new WaterDropSourceTexture());
	sorceTexture_->Init();

	for (int i = 0; i < dropNum_;i++) {
		waterDropSprite_[i].reset(new Sprite(TextureManager::GetInstance()->Load("weightCircle.png"), { 640.0f,360.0f }, 1.0f / 64.0f));
		waterDropSprite_[i]->Initialize();
		waterDropSprite_[i]->SetSize({ 128.0f,128.0f });
		position_[i] = { 640.0f,64.0f };
		radius_[i] = { 64.0f,64.0f };
	}
	for (int i = 0; i < 2;i++) {
		internalEffectTextures_[i].reset(new PostEffect());
		internalEffectTextures_[i]->Init(L"Resources/shaders/WaterDropUpdateEffect.VS.hlsl", L"Resources/shaders/WaterDropUpdateEffect.PS.hlsl");
	}
	latestTextureNum_ = 0;
	isDrawInternal_ = false;
}

void WaterDrop::Update() {
#ifdef _DEBUG
	ImGui::Begin("WaterDrop");
	for (int i = 0; i < dropNum_; i++) {
		char name[32];
		sprintf_s(name,32,"%dposition",i);
		ImGui::DragFloat2(name, &position_[i].x, 1.0f);
		sprintf_s(name, 32, "%dradiusX", i);
		ImGui::DragFloat(name, &radius_[i].x, 1.0f);
		sprintf_s(name, 32, "%dradiusY", i);
		ImGui::DragFloat(name, &radius_[i].y, 1.0f);
		waterDropSprite_[i]->SetSize({radius_[i].x,radius_[i].y});
		waterDropSprite_[i]->SetPosition(position_[i]);
	}
	if (ImGui::Button("DrawInternal"))
	{
		isDrawInternal_ = true;
	}
	ImGui::End();
#endif // _DEBUG
	
	
}

void WaterDrop::DrawInternal() {
	if (isDrawInternal_) {
		sorceTexture_->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
		Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		WaterDropPipeline::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		for (int i = 0; i < dropNum_; i++) {
			waterDropSprite_[i]->Draw();
		}
		sorceTexture_->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
		internalEffectTextures_[latestTextureNum_]->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
		sorceTexture_->Draw(DirectXCommon::GetInstance()->GetCommandList());
		internalEffectTextures_[latestTextureNum_]->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
		isDrawInternal_ = false;
	}
}

void WaterDrop::DrawUpdateEffect() {
	latestTextureNum_ = bool(!latestTextureNum_);
	internalEffectTextures_[latestTextureNum_]->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
	internalEffectTextures_[!latestTextureNum_]->Draw(DirectXCommon::GetInstance()->GetCommandList());
	internalEffectTextures_[latestTextureNum_]->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
}