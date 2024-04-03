#pragma once
#include "Sprite.h"
#include "WaterDropSourceTexture.h"
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
	/// ソーステクスチャに水滴を書き込み
	/// </summary>
	void DrawInternal();

	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return sorceTexture_->GetSrvHandleGPU(); };
private:
	std::unique_ptr<Sprite> waterDropSprite_[10];//水滴用Sprite
	std::unique_ptr<WaterDropSourceTexture> sorceTexture_;//オリジナルのテクスチャの書き込み場所
	static const size_t dropNum_=3;//水滴の数
	Vector2 radius_[dropNum_] = { Vector2{1.0f,1.0f} };
	Vector2 position_[dropNum_];
};
