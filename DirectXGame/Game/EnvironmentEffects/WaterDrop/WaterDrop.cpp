#include "WaterDrop.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "WaterDropPipeline.h"
void WaterDrop::Init() {

	sorceTexture_.reset(new PostEffect());
	sorceTexture_->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	for (int i = 0; i < dropNum_;i++) {
		waterDropSprite_[i].reset(new Sprite(TextureManager::GetInstance()->Load("weightCircle.png"), { 640.0f,360.0f }, 1.0f / 64.0f));
		waterDropSprite_[i]->Initialize();
		waterDropSprite_[i]->SetSize({ 128.0f,128.0f });
		position_[i] = { 640.0f,360.0f };
		radius_[i] = 128.0f;
	}
}

void WaterDrop::Update() {
#ifdef _DEBUG
	ImGui::Begin("WaterDrop");
	for (int i = 0; i < dropNum_; i++) {
		char name[32];
		sprintf_s(name,32,"%dposition",i);
		ImGui::DragFloat2(name, &position_[i].x, 1.0f);
		sprintf_s(name, 32, "%dradius", i);
		ImGui::DragFloat(name, &radius_[i], 1.0f);
		waterDropSprite_[i]->SetSize({radius_[i],radius_[i]});
		waterDropSprite_[i]->SetPosition(position_[i]);
	}
	ImGui::End();
#endif // _DEBUG
	
	
}

void WaterDrop::DrawInternal() {
	sorceTexture_->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
	Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	WaterDropPipeline::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (int i = 0; i < dropNum_; i++) {
		waterDropSprite_[i]->Draw();
	}
	sorceTexture_->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
}