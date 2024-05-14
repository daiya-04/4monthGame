#pragma once
#include "PostEffect.h"

class MagmaDistortion : public PostEffect
{
public:
	/// <summary>
	/// パラメータ
	/// </summary>
	/// <param name="offset">オフセット位置</param>
	/// <param name="roop">ループ(大きいほど変化が細かくなる)</param>
	/// <param name="width">歪み幅、0で歪まない</param>
	struct MagmaData
	{
		float offset; //オフセット位置
		float roop; //ループ(大きいほど変化が細かくなる)
		float width; //歪み幅、0で歪まない
		float magmaLine;//マグマの上端の位置(スクリーン座標)
	};

	MagmaDistortion() = default;
	~MagmaDistortion() = default;

	void Init() override;
	void Draw(ID3D12GraphicsCommandList* cmdList) override;

	void SetOffset(float offset) {
		magmaData_->offset = offset;
	};
	void SetRoop(float roop) {
		magmaData_->roop = roop;
	};
	void SetWidth(float width) {
		magmaData_->width = width;
	};

	void SetMagmaLine(float line) {
		magmaData_->magmaLine = line;
	}

private:
	void CreateGraphicsPipelineState() override;

	ComPtr<ID3D12Resource> magmaBuff_;
	MagmaData* magmaData_ = nullptr;
};
