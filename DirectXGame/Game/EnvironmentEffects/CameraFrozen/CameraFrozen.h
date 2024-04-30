#pragma once
#include "PostEffect.h"

class CameraFrozen : public PostEffect
{
public:
	/// <summary>
	/// パラメータ
	/// </summary>
	/// <param name="offset">オフセット位置</param>
	/// <param name="roop">ループ(大きいほど変化が細かくなる)</param>
	/// <param name="width">歪み幅、0で歪まない</param>
	struct CameraFrozenData
	{
		float radius; //半径
		float border; //書き込み確率(0~1,0で確定)
		float seed;//乱数のシード(適当な値)
		int32_t mode;//0->凍結 1->解凍
	};

	CameraFrozen() = default;
	~CameraFrozen() = default;

	void Init() override;
	void DrawInternal(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	/// RenderTargetをclearしないPreDraw処理
	/// </summary>
	void PreDrawNotClearRTV(ID3D12GraphicsCommandList* cmdList);
	void CrearRenderTarget(ID3D12GraphicsCommandList* cmdList) {
		cmdList->ClearRenderTargetView(rtvHandleCPU_, clearColor_, 0, nullptr);
	};
	void SetRadius(float radius) {
		cameraFrozenData_->radius = radius;
	};
	void SetBorder(float border) {
		cameraFrozenData_->border = border;
	};

	void SetSeed(float seed) {
		cameraFrozenData_->seed = seed;
	};
	void SetMode(int32_t mode) {
		cameraFrozenData_->mode = mode;
	}

	int32_t GetMode() {
		return cameraFrozenData_->mode;
	}

private:
	/// <summary>
	/// 自らに書き込むためのパイプライン作成関数
	/// </summary>
	void CreateGraphicsPipelineStateInternal();

	ComPtr<ID3D12PipelineState> piplineStateInternal_;
	ComPtr<ID3D12RootSignature> rootSignatureInternal_;

	ComPtr<ID3D12Resource> cameraFrozenBuff_;
	CameraFrozenData* cameraFrozenData_ = nullptr;
};
