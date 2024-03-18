#include "CameraFrozenManager.h"
#include "ImGuiManager.h"
void CameraFrozenManager::Init() {
	isClearRender_ = true;
	isActive_ = false;
	radius_ = 0.8f;
	border_ = 0.9f;
	radiusMax_ = 0.8f;
	radiusMin_ = 0.5f;
	seed_ = 0.0f;
	cameraFrozen_.reset(new CameraFrozen());
	cameraFrozen_->Init();
	cameraFrozen_->SetRadius(radius_);
	cameraFrozen_->SetBorder(border_);
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
		if (radius_> radiusMin_) {
			radius_ -= 0.005f;
		}
		else {
			isActive_ = false;
			radius_ = radiusMin_;
		}
	}
	seed_+=0.01f;
	cameraFrozen_->SetRadius(0.65f);
	cameraFrozen_->SetBorder(border_);
	cameraFrozen_->SetSeed(seed_);
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