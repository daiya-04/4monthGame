#pragma once
#include "Sprite.h"
#include "PostEffect.h"
#include <memory>
class WaterDrop
{
public:
	/// <summary>
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
	void DrawInternal();

	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return sorceTexture_->GetSrvHandleGPU(); };
private:
	std::unique_ptr<Sprite> waterDropSprite_[10];//水滴一個
	std::unique_ptr<PostEffect> sorceTexture_;//オリジナルのテクスチャの書き込み場所
	static const size_t dropNum_=2;//水滴の数
	float radius_[dropNum_] = { 1.0f };
	Vector2 position_[dropNum_];
};
