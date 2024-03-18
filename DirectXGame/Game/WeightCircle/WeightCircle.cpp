#include "WeightCircle.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
void WeightCircle::Init() {

	weight_.reset(new PostEffect());
	weight_->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	circle_.reset(new Sprite(TextureManager::GetInstance()->Load("weightCircle.png"), { 640.0f,360.0f }, 1.0f/64.0f));
	circle_->Initialize();
	position_ = { 640.0f,360.0f};
	radius_ = 0.0f;
	weightColor_ = 1.0f;
	isStart_ = false;
}

void WeightCircle::Update() {
#ifdef _DEBUG
	ImGui::Begin("weightCircle");
	ImGui::DragFloat2("position",&position_.x,1.0f);
	ImGui::DragFloat("radius", &radius_, 1.0f);
	ImGui::SliderFloat("weight", &weightColor_, 0.0f, 1.0f);
	if (ImGui::Button("Start"))
	{
		isStart_ = true;
		radius_ = 0.0f;
	}
	if (ImGui::Button("Reset"))
	{
		isStart_ = false;
		radius_ = 0.0f;
	}
	ImGui::End();
#endif // _DEBUG
	if (isStart_) {
		radius_ += 33.0f;
	}
	circle_->SetColor({weightColor_,0.0f,0.0f,1.0f});
	circle_->SetSize({radius_,radius_ });
	circle_->SetPosition(position_);
}

void WeightCircle::Draw() {
	weight_->PreDrawScene(DirectXCommon::GetInstance()->GetCommandList());
	Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	circle_->Draw();
	Sprite::postDraw();
	weight_->PostDrawScene(DirectXCommon::GetInstance()->GetCommandList());
}