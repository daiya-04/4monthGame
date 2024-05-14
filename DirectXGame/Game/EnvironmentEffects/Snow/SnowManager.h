#pragma once
//#include "Sprite.h"
#include "Object2d.h"
#include "PostEffect.h"
#include "SnowEffectDrawer.h"
#include "SnowObstacle.h"
#include <memory>
class SnowManager
{
public:

	static SnowManager* GetInstance() {
		static SnowManager instance;
		return &instance;
	}

	/// <summary>
	/// 初期化
	/// </summary>
	void Init();
	/// <summary>
	/// 更新
	/// </summary>
	//void Update();
	/// <summary>
	
	/// <summary>
	/// エフェクトの更新
	/// </summary>
	void PreDrawUpdateEffect();

	/// <summary>
	/// エフェクトの更新
	/// </summary>
	/// <param name="camera">障害物の描画に使ったものと同じカメラ</param>
	void PostDrawUpdateEffect(const Camera& camera);

	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetHandle() { return internalEffectTextures_[latestTextureNum_]->GetSrvHandleGPU(); };

	void Draw();

	void SetCameraSlide(const Vector2& vec) { cameraSlide_ = vec; };

private:

	SnowManager() { Init(); };

	std::unique_ptr<SnowObstacle> internalEffectTextures_[2];//加工後の水滴テクスチャ、書き込みごとに入れ替わる
	size_t latestTextureNum_;//書き込み先のテクスチャ番号(0 or 1)
	
	std::unique_ptr<Object2d> originalSnow;//雪の位置
	std::unique_ptr<PostEffect> snowPositionTexture_;//雪単体が書き込まれたテクスチャ
	bool isDrawUpdate_;//エフェクトの更新を行うか
	std::unique_ptr<SnowEffectDrawer> snowEffectDrawer_;//雪単体が書き込まれたテクスチャ
	bool isClearEffect_=true;//障害物除去
	Vector2 cameraSlide_;
};
