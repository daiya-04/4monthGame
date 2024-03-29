#pragma once
#include <memory>
#include "Object2d.h"
#include "Vec2.h"
#include "Camera.h"
#include "Input.h"
#include "PlayerParameter.h"
#include <array>
#include "Hit.h"

class Player
{
public:
	Player();
	~Player();

	/// <summary>
	/// 4頂点、中心のどれかを指す
	/// </summary>
	enum Point {
		kLeftTop, //左上
		kRightTop, //右上
		kLeftBottom, //左下
		kRightBottom, //右下
		kCenter, //中央
	};

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

	void SetPosition(const Vector2& position) { 
		position_ = position;
		//4頂点の座標を更新
		leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
		rightTop_ = { position_.x + kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
		leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };
		rightBottom_ = { position_.x + kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };
	}

	/// <summary>
	/// プレイヤーの中心座標、または4頂点のいずれかを返す
	/// </summary>
	/// <param name="point">取得したい座標</param>
	/// <returns></returns>
	const Vector2& GetPosition(Point point = kCenter) {

		switch (point)
		{
		case Player::kLeftTop:
			return leftTop_;
			break;
		case Player::kRightTop:
			return rightTop_;
			break;
		case Player::kLeftBottom:
			return leftBottom_;
			break;
		case Player::kRightBottom:
			return rightBottom_;
			break;
		default:
		case Player::kCenter:
			return position_;
			break;
		}

		return position_;

	}

	/// <summary>
	/// プレイヤーの前フレームの中心座標、または4頂点を返す
	/// </summary>
	/// <param name="point">取得したい座標</param>
	/// <returns></returns>
	const Vector2& GetPrePosition(Point point = kCenter) {

		switch (point)
		{
		case Player::kLeftTop:
			return preLeftTop_;
			break;
		case Player::kRightTop:
			return preRightTop_;
			break;
		case Player::kLeftBottom:
			return preLeftBottom_;
			break;
		case Player::kRightBottom:
			return preRightBottom_;
			break;
		default:
		case Player::kCenter:
			return prePosition_;
			break;
		}

		return prePosition_;

	}

	Vector2* GetPositionPtr() { return &position_; }

	const AABB2D& GetCollision() { return collision_; }

	void SetIsFly(bool flag) { isFly_ = flag; }

	void SetCanJump(bool flag) { parameter_.Jump_.canJump = flag; }

	void ResetVelocityY() { velocity_.y = 0.0f; }

private:

	void Move();

	void Jump();

	void WallJump();

private:

	Input* input_;

	std::unique_ptr<Object2d> object_;

	//当たり判定
	AABB2D collision_{};

	//体温の最高値
	/*const uint32_t kMaxTemperature_ = 140;*/

	//体温の最低値
	/*const uint32_t kMinTemperature_ = 80;*/

	//体温
	/*uint32_t temperature_ = kMaxTemperature_;*/

	//水分量の最大値
	/*const uint32_t kMaxBodyMoisture_ = 100;*/

	//水分量の最低値
	/*const uint32_t kMinBodyMoisture_ = 0;*/

	//水分量
	/*uint32_t bodyMoisture_ = 50;*/

	//パラメータを纏めたもの
	PlayerParameter parameter_;

	//空中判定
	bool isFly_ = true;

	//落下速度下限
	const float maxFallSpeed_ = 15.0f;

	//速度
	Vector2 velocity_{};

	//加速度
	Vector2 accel_{};

	//位置
	Vector2 position_{};

	//前フレームの位置
	Vector2 prePosition_{};

	//左上
	Vector2 leftTop_{};
	//右上
	Vector2 rightTop_{};
	//左下
	Vector2 leftBottom_{};
	//右下
	Vector2 rightBottom_{};

	//前フレーム左上
	Vector2 preLeftTop_{};
	//前フレーム右上
	Vector2 preRightTop_{};
	//前フレーム左下
	Vector2 preLeftBottom_{};
	//前フレーム右下
	Vector2 preRightBottom_{};

	//プレイヤー画像
	uint32_t texture_;

	//デバッグフラグ
	bool isDebug_ = false;

	//当たり判定を使うかどうか
	bool useCollision_ = true;

};


