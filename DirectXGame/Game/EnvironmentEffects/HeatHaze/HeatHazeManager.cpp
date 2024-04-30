#include "HeatHazeManager.h"
#include "ImGuiManager.h"
void HeatHazeManager::Init() {
	heatHaze_.reset(new HeatHaze());
	heatHaze_->Init();
	offset_ = 0;
	roop_ = 2.3f;
	width_ = 0.003f;
	effectTexture_.reset(new PostEffect());
	effectTexture_->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");
}

void HeatHazeManager::Update() {
#ifdef _DEBUG
	ImGui::Begin("HeatHaze");
	ImGui::DragFloat("offset", &offset_, 0.01f);
	ImGui::DragFloat("roop", &roop_, 0.1f);
	ImGui::DragFloat("width", &width_, 0.0001f);
	ImGui::End();
#endif // _DEBUG
	offset_ += 0.001f;
	heatHaze_->SetOffset(offset_);
	heatHaze_->SetRoop(roop_);
	heatHaze_->SetWidth(width_);
}