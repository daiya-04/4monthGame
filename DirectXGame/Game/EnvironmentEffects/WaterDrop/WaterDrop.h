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
	/// 加工後テクスチャに書き込み(エフェクトの更新)
	/// </summary>
	void DrawUpdateEffect();

	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return internalEffectTextures_[latestTextureNum_]->GetSrvHandleGPU(); };

	bool GetIsDrawInternal() { return isDrawInternal_; };

private:
	std::unique_ptr<WaterDropSourceTexture> sorceTexture_;//オリジナルのテクスチャの書き込み場所
	std::unique_ptr<PostEffect> internalEffectTextures_[2];//加工後の水滴テクスチャ、書き込みごとに入れ替わる
	size_t latestTextureNum_;//書き込み先のテクスチャ番号(0 or 1)
	static const size_t dropNum_=10;//水滴の数
	bool isDrawInternal_;//オリジナルテクスチャの書き込みを行うか
	Vector2 radius_[dropNum_] = { Vector2{1.0f,1.0f} };
	Vector2 position_[dropNum_];
	std::unique_ptr<Sprite> waterDropSprite_[dropNum_];//水滴用Sprite
};
