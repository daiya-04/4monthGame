#include "CameraFrozenManager.h"
#include "ImGuiManager.h"
#include <algorithm>
void CameraFrozenManager::Init() {
	isClearRender_ = true;
	isActive_ = false;
	radius_ = 0.8f;
	border_ = 0.9f;
	radiusMax_ = 1.0f;
	radiusMin_ = 0.7f;
	radiusMinTarget_ = radiusMin_;
	seed_ = 0.0f;
	cameraFrozen_.reset(new CameraFrozen());
	cameraFrozen_->Init();
	cameraFrozen_->SetRadius(radius_);
	cameraFrozen_->SetBorder(border_);
	cameraFrozen_->SetMode(1);
}

void CameraFrozenManager::Update() {
#ifdef _DEBUG
	ImGui::Begin("CameraFrozen");
	ImGui::DragFloat("border",&border_,0.01f,0.0f,1.0f);
	ImGui::DragFloat("radiusMin", &radiusMin_, 0.01f, 0.0f, 1.0f);
	ImGui::DragFloat("radiusMax", &radiusMax_, 0.01f, 0.0f, 1.0f);
	if (ImGui::Button("Start"))
	{
		isActive_ = true;
		radius_ = radiusMax_;
		isClearRender_ = true;
	}
	//ImGui::RadioButton("IsActive", isActive_);
	ImGui::End();
#endif // _DEBUG
	if (isActive_) {
		
		if(std::fabsf(radius_ - radiusMin_)<0.004f) {
			isActive_ = false;
			//radius_ = radiusMin_;
			if (cameraFrozen_->GetMode()) {
				isClearRender_=true;
			}
		}
		else if (radius_ > radiusMin_) {
			radius_ -= 0.003f;
		}
		else {
			radius_ += 0.003f;
		}
	}
	else if(preRadius_ != radiusMin_){
		if (!cameraFrozen_->GetMode()) {
			isActive_ = true;
			radius_ = radiusMin_;
		}
	}
	seed_+=0.01f;
	cameraFrozen_->SetRadius(radiusMin_);
	cameraFrozen_->SetBorder(border_);
	cameraFrozen_->SetSeed(seed_);
	/*if (radiusMinTarget_ - radiusMin_ < 0.005f) {
		radiusMin_ += 0.01f;
	}
	else if (radiusMinTarget_ - radiusMin_>-0.005f) {
		radiusMin_ -= 0.01f;
	}*/
	preRadius_ = radiusMin_;
}

void CameraFrozenManager::Draw(ID3D12GraphicsCommandList* cmdList) {
	cameraFrozen_->Draw(cmdList);
}

void CameraFrozenManager::DrawInternal(ID3D12GraphicsCommandList* cmdList) {
	cameraFrozen_->PreDrawNotClearRTV(cmdList);
	if (isClearRender_) {
		cameraFrozen_->CrearRenderTarget(cmdList);
		isClearRender_ = false;
	}
	if (isActive_) {
		cameraFrozen_->DrawInternal(cmdList);
	}
	cameraFrozen_->PostDrawScene(cmdList);
}

void CameraFrozenManager::CulcFrozenArea(float playerHeight, float magmaBorder) {
	if (!cameraFrozen_->GetMode()) {
		float dist = magmaBorder- playerHeight;
		dist = std::clamp((dist/720.0f), 0.0f, 1.0f);
		radiusMin_ = (1.0f - dist) * 0.55f + dist * 0.75f;
	}
}