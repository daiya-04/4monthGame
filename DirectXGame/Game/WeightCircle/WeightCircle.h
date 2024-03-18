#pragma once
#include <memory>
#include "PostEffect.h"
#include "Sprite.h"
class WeightCircle {
public:/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// 描画(マップに書き込み)
	/// </summary>
	void Draw();
	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return weight_->GetSrvHandleGPU(); };

private:
	std::unique_ptr<PostEffect> weight_;
	std::unique_ptr<Sprite> circle_;
	float radius_=1.0f;
	Vector2 position_;
	float weightColor_;
	bool isStart_;
};