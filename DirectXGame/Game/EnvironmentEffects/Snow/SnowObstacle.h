#pragma once
#include "PostEffect.h"

class SnowObstacle : public PostEffect
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

	SnowObstacle() = default;
	~SnowObstacle() = default;

	void Init() override;
	
	
	void Draw(ID3D12GraphicsCommandList* cmdList);
	
	void SetCamelaSlide(const Vector2& vec) { (*cameraSlideData_) = vec; };

private:
	void CreateGraphicsPipelineState() override;
	//カメラ平行移動
	ComPtr<ID3D12Resource> cameraSlideBuff_;
	Vector2* cameraSlideData_ = nullptr;
};
