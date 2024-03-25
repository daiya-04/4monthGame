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

private:

	//スクロールを追従させるターゲットの座標
	Vector2* target_ = nullptr;

	//カメラのポインタ
	Camera* camera_ = nullptr;

	//スクロール値
	Vector2 scroll_{};

	//スクロール制限
	Vector2 limit_{Stage::kMaxStageWidth_ * Block::kBlockSize_ / 4, Stage::kMaxStageHeight_ * Block::kBlockSize_ };

	//スクロール開始座標
	Vector2 startScroll_{ 640.0f,360.0f };

};
