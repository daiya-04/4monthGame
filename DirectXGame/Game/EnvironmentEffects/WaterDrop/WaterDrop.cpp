#include "WaterDrop.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "WaterDropPipeline.h"
#include "RandomEngine/RandomEngine.h"
void WaterDrop::Init() {

	sorceTexture_.reset(new WaterDropSourceTexture());
	sorceTexture_->Init();

	for (int i = 0; i < dropNum_;i++) {
		waterDropSprite_[i].reset(new Sprite(TextureManager::GetInstance()->Load("weightCircle.png"), { 640.0f,360.0f }, 1.0f / 64.0f));
		waterDropSprite_[i]->Initialize();
		waterDropSprite_[i]->SetSize({ 128.0f,128.0f });
		float length = RandomEngine::GetRandom(0.7f, 1.4f);
		Vector2 dir = { RandomEngine::GetRandom(-1.0f,1.0f),RandomEngine::GetRandom(-1.0f,1.0f) };
		dir = dir.Normalize();
		Vector2 halfWinSize = {640.0f,360.0f};
		position_[i] = { halfWinSize .x*(dir.x*length) + halfWinSize.x,halfWinSize.y * (dir.y * length)+ halfWinSize.y };
		position_[i].x = std::clamp(position_[i].x, 8.0f, 1280.0f - 8.0f);
		position_[i].y = std::clamp(position_[i].y, 8.0f,  720.0f - 8.0f);
		radius_[i] = { 6.0f,8.0f };
	}
	for (int i = 0; i < 2;i++) {
		internalEffectTextures_[i].reset(new PostEffect());
		internalEffectTextures_[i]->Init(L"Resources/shaders/WaterDropUpdateEffect.VS.hlsl", L"Resources/shaders/WaterDropUpdateEffect.PS.hlsl");
	}
	SetPositionRandom();
	latestTextureNum_ = 0;
	isDrawInternal_ = true;
	isDrawUpdate_ = false;
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
		isDrawUpdate_ = false;
		SetPositionRandom();
	}
	if (ImGui::Button("Start"))
	{
		isDrawUpdate_ = true;
	}
	if (ImGui::Button("Stop"))
	{
		isDrawUpdate_ = false;
	}
	ImGui::End();
#endif // _DEBUG
	
	
}

void WaterDrop::Reset() {
	isDrawInternal_ = true;
	isDrawUpdate_ = false;
	SetPositionRandom();
}

void WaterDrop::Start() {
	isDrawUpdate_ = true;
}

void WaterDrop::DrawInternal() {
	if (isDrawInternal_ ) {
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
	if (isDrawUpdate_) {
		latestTextureNum_ = bool(!latestTextureNum_);
		internalEffectTextures_[latestTextureNum_]->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
		internalEffectTextures_[!latestTextureNum_]->Draw(DirectXCommon::GetInstance()->GetCommandList());
		internalEffectTextures_[latestTextureNum_]->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
	}
}

void WaterDrop::SetPositionRandom() {
	for (int i = 0; i < dropNum_; i++) {
		float length = RandomEngine::GetRandom(0.8f, 1.4f);
		Vector2 dir = { RandomEngine::GetRandom(-1280.0f,1280.0f),RandomEngine::GetRandom(-720.0f,720.0f) };
		//Vector2 normalizedDir = dir.Normalize();
		Vector2 halfWinSize = { 640.0f,360.0f };
		//dir.x = normalizedDir.x * (halfWinSize.x / halfWinSize.y);
		dir = dir.Normalize();
		//dir.y = normalizedDir.y * (halfWinSize.y / halfWinSize.x);
		position_[i] = { halfWinSize.x * (dir.x * length) + halfWinSize.x,halfWinSize.y * (dir.y * length) + halfWinSize.y };
		position_[i].x = std::clamp(position_[i].x, 8.0f, 1280.0f - 8.0f);
		position_[i].y = std::clamp(position_[i].y, 8.0f, 720.0f - 8.0f);
	}
}