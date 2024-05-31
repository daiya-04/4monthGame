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

	void Start() { isStart_ = true; };
	void Reset() { 
		isStart_ = false;
		isEnd_ = false;
		radius_ = 0.0f;
	};

	bool GetIsStart() { return isStart_; };
	bool GetIsEnd() { return isEnd_; };

	void SetPosition(const Vector2& pos) { position_ = pos; };

private:
	std::unique_ptr<PostEffect> weight_;
	std::unique_ptr<Sprite> circle_;
	float radius_=1.0f;
	Vector2 position_;
	float weightColor_;
	bool isStart_;
	bool isEnd_;
};