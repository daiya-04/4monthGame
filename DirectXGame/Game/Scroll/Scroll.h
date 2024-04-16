#pragma once
#include "Vec2.h"
#include "Game/Stage/Stage.h"
#include "Camera.h"

/// <summary>
/// スクロール管理クラス
/// </summary>
class Scroll
{
public:
	Scroll();
	~Scroll();

	void Initialize();

	void Update();

	void SetTarget(Vector2* target) { target_ = target; }

	void SetCamera(Camera* camera) { camera_ = camera; }

	//画面の中心座標
	static inline const Vector2 kWindowCenter_ = { 640.0f,360.0f };

	//スクロール制限始点
	static inline Vector2 limitStart_{ 0.0f,-1000.0f };

	//スクロール制限終点
	static inline Vector2 limitEnd_{ Stage::kMaxStageWidth_ * Block::kBlockSize_ - kWindowCenter_.x - Block::kBlockSize_,
		Stage::kMaxStageHeight_ * Block::kBlockSize_ - kWindowCenter_.y - Block::kBlockSize_ };

private:

	//スクロールを追従させるターゲットの座標
	Vector2* target_ = nullptr;

	//カメラのポインタ
	Camera* camera_ = nullptr;

	//スクロール値
	Vector2 scroll_{};

	//スクロール開始座標
	Vector2 startScroll_{ limitStart_.x + kWindowCenter_.x,limitStart_.y + kWindowCenter_.y };

};
