#pragma once
#include "Object2d.h"
#include "Vec2.h"
#include "Camera.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include <memory>
#include "Hit.h"
#include "Block/Block.h"
#include <array>
#include "Stage/StageConstant.h"

class BaseEnemy
{
public:

	BaseEnemy();
	~BaseEnemy() = default;

	static const int32_t kEnemyHalfSize_ = 32;

	virtual void Initialize(const Vector2& position) = 0;

	virtual void Update() = 0;

	virtual void Draw(const Camera& camera) = 0;

	const AABB2D& GetCollision() { return collision_; }

	void SetBlocks(std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* blocks) { blocksPtr_ = blocks; }

	void SetPosition(const Vector2& position) {
		position_ = position;
		//4頂点の座標を更新
		leftTop_ = { position_.x - kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
		rightTop_ = { position_.x + kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
		leftBottom_ = { position_.x - kEnemyHalfSize_, position_.y + kEnemyHalfSize_ };
		rightBottom_ = { position_.x + kEnemyHalfSize_, position_.y + kEnemyHalfSize_ };
		//当たり判定更新
		collision_.min = { position_.x - kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
		collision_.max = { position_.x + kEnemyHalfSize_, position_.y + kEnemyHalfSize_ };

	}

	void SetTmpPosition(const Vector2& position) {
		tmpPosition_ = position;
		//4頂点の座標を更新
		leftTop_ = { tmpPosition_.x - kEnemyHalfSize_, tmpPosition_.y - kEnemyHalfSize_ };
		rightTop_ = { tmpPosition_.x + kEnemyHalfSize_ - 1, tmpPosition_.y - kEnemyHalfSize_ };
		leftBottom_ = { tmpPosition_.x - kEnemyHalfSize_, tmpPosition_.y + kEnemyHalfSize_ - 1 };
		rightBottom_ = { tmpPosition_.x + kEnemyHalfSize_ - 1, tmpPosition_.y + kEnemyHalfSize_ - 1 };
		//当たり判定更新
		collision_.min = { tmpPosition_.x - kEnemyHalfSize_, tmpPosition_.y - kEnemyHalfSize_ };
		collision_.max = { tmpPosition_.x + kEnemyHalfSize_ - 1, tmpPosition_.y + kEnemyHalfSize_ - 1 };

	}

protected:

	void CheckCollision();

protected:

	//ブロックの配列ポインタ
	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* blocksPtr_ = nullptr;

	Vector2 velocity_{};

	std::unique_ptr<Object2d> object_;

	AABB2D collision_{};

	//右に移動するか左に移動するか
	bool isMoveLeft_ = false;

	float maxFallVelocity_ = 25.0f;

	//自然落下速度
	float gravityFallSpeed_ = 2.0f;

	//位置
	Vector2 position_{};

	//移動予定の位置
	Vector2 tmpPosition_{};

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

	uint32_t enemyTexture_;

};

class NormalEnemy : public BaseEnemy 
{
public:

	void Initialize(const Vector2& position) override;

	void Update() override;

	void Draw(const Camera& camera) override;

private:

};
