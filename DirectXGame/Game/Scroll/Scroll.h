#pragma once
#include "Vec2.h"
#include "Game/Stage/StageConstant.h"
#include "Camera.h"
#include <array>
#include "Block/Block.h"

/// <summary>
/// スクロール管理クラス
/// </summary>
class Scroll
{
public:
	Scroll();
	~Scroll();

	enum ScrollType {
		kNomral, //完全追従
		kDelay, //遅延
		kMaxScrollType, //スクロールの種類
	};

	void Initialize();

	void Update();

	void SetTarget(Vector2* target) { target_ = target; }

	void SetCamera(Camera* camera) { camera_ = camera; }

	void SetIsSlow(bool flag) { isSlow_ = flag; }

	void SetCameraOnTarget();

	/// <summary>
	/// スクロールの種類切り替え
	/// </summary>
	/// <param name="index">X座標なら0, Yなら1</param>
	/// <param name="type">スクロールの種類</param>
	void SetScrollType(int32_t index, ScrollType type) { 
		index = std::clamp(index, 0, 1);
		type_[index] = type;
	}

	//画面の中心座標
	static inline Vector2 kWindowCenter_ = { 800.0f,450.0f };

	//スクロール制限始点
	static inline Vector2 limitStart_{ 0.0f,-2000.0f };

	//スクロール制限終点
	static inline Vector2 limitEnd_{ kMaxStageWidth * Block::kBlockSize_ - kWindowCenter_.x - Block::kBlockSize_,
		kMaxStageHeight * Block::kBlockSize_ - kWindowCenter_.y - Block::kBlockSize_ };

private:

	//スクロールを追従させるターゲットの座標
	Vector2* target_ = nullptr;

	//カメラのポインタ
	Camera* camera_ = nullptr;

	//スクロール値
	Vector2 scroll_{};

	//スクロール開始座標
	Vector2 startScroll_;

	//スクロールの種類
	std::array<ScrollType, 2> type_;

	//カメラ補正のスピード
	float stalkerSpeed_ = 0.2f;
	//リスに戻るまでのカメラスピード
	float respawnStalkerSpeed_ = 0.04f;
	//ストーカースピードの切り替え
	bool isSlow_ = false;

};
