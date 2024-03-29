#pragma once
#include <memory>
#include "Object2d.h"
#include "Vec2.h"
#include "Camera.h"
#include "Input.h"
#include "PlayerParameter.h"
#include <array>

class Player
{
public:
	Player();
	~Player();

	//プレイヤーのサイズ
	static const uint32_t kPlayerSize_ = 96;
	//プレイヤーサイズの半分
	static const uint32_t kPlayerHalfSize_ = kPlayerSize_ / 2;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

	/// <summary>
	/// デバッグ描画
	/// </summary>
	void Debug();

	void SetPosition(const Vector2& position) { position_ = position; }

	const Vector2& GetPosition() { return position_; }

	Vector2* GetPositionPtr() { return &position_; }

private:

	void Move();

	void Jump();

	void WallJump();

private:

	Input* input_;

	std::unique_ptr<Object2d> object_;

	//体温の最高値
	const uint32_t kMaxTemperature_ = 140;

	//体温の最低値
	const uint32_t kMinTemperature_ = 80;

	//体温
	uint32_t temperature_ = kMaxTemperature_;

	//水分量の最大値
	/*const uint32_t kMaxBodyMoisture_ = 100;*/

	//水分量の最低値
	/*const uint32_t kMinBodyMoisture_ = 0;*/

	//水分量
	/*uint32_t bodyMoisture_ = 50;*/

	//パラメータを纏めたもの
	PlayerParameter parameter_;

	//空中判定
	bool isFly_ = false;

	//落下速度下限
	const float maxFallSpeed_ = 15.0f;

	//速度
	Vector2 velocity_{};

	//加速度
	Vector2 accel_{};

	//位置
	Vector2 position_{};

	//左上
	Vector2 leftTop_{};
	//右上
	Vector2 rightTop_{};
	//左下
	Vector2 leftBottom_{};
	//右下
	Vector2 rightBottom_{};

	//プレイヤー画像
	uint32_t texture_;

	bool isDebug_ = true;

};


