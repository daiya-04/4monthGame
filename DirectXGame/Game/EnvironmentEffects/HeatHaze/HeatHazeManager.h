#pragma once
#include "HeatHaze.h"
#include <memory>
class HeatHazeManager {
public:
	static HeatHazeManager* GetInstance() {
		static HeatHazeManager instance;
		return &instance;
	}
	void Init();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList) { heatHaze_->Draw(cmdList); };
	void PreDraw(ID3D12GraphicsCommandList* cmdList) { heatHaze_->PreDrawScene(cmdList); };
	void PostDraw(ID3D12GraphicsCommandList* cmdList) { heatHaze_->PostDrawScene(cmdList); };
	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return heatHaze_->GetSrvHandleGPU(); };
private:
	HeatHazeManager() {
		Init();
	}
	float offset_;
	float roop_;
	float width_;
	std::unique_ptr<HeatHaze> heatHaze_;
};