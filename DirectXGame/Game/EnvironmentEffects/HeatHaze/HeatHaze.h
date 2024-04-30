#pragma once
#include "PostEffect.h"

class HeatHaze : public PostEffect
{
public:
	/// <summary>
	/// パラメータ
	/// </summary>
	/// <param name="offset">オフセット位置</param>
	/// <param name="roop">ループ(大きいほど変化が細かくなる)</param>
	/// <param name="width">歪み幅、0で歪まない</param>
	struct HeatHazeData
	{
		float offset; //オフセット位置
		float roop; //ループ(大きいほど変化が細かくなる)
		float width; //歪み幅、0で歪まない
	};

	HeatHaze()=default;
	~HeatHaze() = default;

	void Init() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

	void SetOffset(float offset) {
		heatHazeData_->offset = offset;
	};
	void SetRoop(float roop) {
		heatHazeData_->roop = roop;
	};
	void SetWidth(float width) {
		heatHazeData_->width = width;
	};

private:
	void CreateGraphicsPipelineState() override;

	ComPtr<ID3D12Resource> heatHazeBuff_;
	HeatHazeData* heatHazeData_ = nullptr;
};
