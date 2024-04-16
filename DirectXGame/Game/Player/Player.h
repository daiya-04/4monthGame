#pragma once
#include <memory>
#include "Object2d.h"
#include "Vec2.h"
#include "Camera.h"
#include "Input.h"
#include "PlayerParameter.h"
#include <array>
#include "Hit.h"
#include <vector>
#include "Stage/Stage.h"
#include <list>
#include "Sprite.h"

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

	/// <summary>
	/// 現在使っているキャラクターがどっちかを指す
	/// </summary>
	enum Characters : uint32_t {
		kLeftPlayer, //左から出るプレイヤー
		kRightPlayer, //右から出るプレイヤー

		kMaxPlayer, //最大人数

	};

	//現在操作しているプレイヤーに加算するパラメータ値
	struct AddParameters {
		//スピード
		float addSpeed = 0.0f;
		//採掘速度
		int32_t addDigSpeed = 0;
		//耐熱時間
		int32_t addSaunaTime = 0;
	};

	//ブロック1つ当たりの加算量
	struct AddValue {
		//スピード
		float speed = 2.0f;
		//採掘速度
		int32_t digSpeed = 5;
		//耐熱時間
		int32_t saunaTime = 100;
	};

	//プレイヤーのサイズ
	static const uint32_t kPlayerSize_ = 64;
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
	/// UI描画
	/// </summary>
	void DrawUI();

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
		//当たり判定更新
		collision_.min = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
		collision_.max = { position_.x + kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };

	}

	void SetTmpPosition(const Vector2& position) {
		tmpPosition_ = position;
		//4頂点の座標を更新
		leftTop_ = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y - kPlayerHalfSize_ };
		rightTop_ = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y - kPlayerHalfSize_ };
		leftBottom_ = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y + kPlayerHalfSize_ - 1 };
		rightBottom_ = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y + kPlayerHalfSize_ - 1 };
		//当たり判定更新
		collision_.min = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y - kPlayerHalfSize_ };
		collision_.max = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y + kPlayerHalfSize_ - 1 };

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

	Vector2* GetBirdsEyePositionPtr() { return &birdsEyePosition_; }

	const AABB2D& GetCollision() { return collision_; }

	void SetCanJump(bool flag) { parameters_[currentCharacters_]->Jump_.canJump = flag; }

	void ResetVelocityY() { velocity_.y = 0.0f; }

	void SetBlocks(std::array<std::array<std::shared_ptr<Block>, Stage::kMaxStageWidth_>, Stage::kMaxStageHeight_>* blocks) { blocksPtr_ = blocks; }

	bool GetIsDead() const { return isDead_; }

	bool GetIsBirdsEye() const { return isBirdsEye_; }

private:

	//移動
	void Move();

	//ジャンプ
	void Jump();

	//壁キック
	void WallJump();

	//溜めジャンプ
	void ChargeJump();

	//穴掘り
	void Dig();

	//座標更新
	void UpdatePosition();

	//当たり判定チェック
	void CheckCollision();

	//プレイヤー強化
	void Enhance();

	//キャラクター交代
	void Change();

	//現在拠点にいる状態にするための設定
	void SetOnBase();

	//サウナ室にいる方の残り時間カウント
	void CountSaunaTime();

private:

	Input* input_;

	std::unique_ptr<Object2d> object_;

	//ブロックの配列ポインタ
	std::array<std::array<std::shared_ptr<Block>, Stage::kMaxStageWidth_>, Stage::kMaxStageHeight_>* blocksPtr_ = nullptr;

	//当たり判定
	AABB2D collision_{};

	//デフォルトパラメータ。調整、初期化用
	std::unique_ptr<PlayerParameter> defaultParameter_;

	//パラメータを纏めたもの
	std::array<std::unique_ptr<PlayerParameter>, kMaxPlayer> parameters_;

	//パラメータ値加算量を蓄積するもの
	AddParameters addParameters_;

	//一ブロック毎の加算量
	AddValue addValue_;

	//落下速度下限
	const float kMaxFallSpeed_ = 25.0f;

	//自然落下速度
	const float kGravityFallSpeed_ = 2.0f;

	//速度
	Vector2 velocity_{};

	//壁キックによる速度加算
	Vector2 wallJumpVelocity_{};

	//加速度
	Vector2 accel_{};

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

	//俯瞰視点のターゲット座標
	Vector2 birdsEyePosition_{};

	//プレイヤー画像
	uint32_t texture_;
	uint32_t textureLeft_;
	uint32_t textureRight_;
	uint32_t textureUp_;
	uint32_t textureDown_;

	//デバッグフラグ
	bool isDebug_ = false;

	//当たり判定を使うかどうか
	bool useCollision_ = true;

	//プレイヤーの向き
	bool isFacingLeft_ = true;

	//現在使用しているプレイヤー
	Characters currentCharacters_ = kLeftPlayer;

	//サウナ室に入っているプレイヤー
	Characters reserveCharacters_ = kRightPlayer;

	//採掘開始フラグ
	bool isMining_ = true;

	//拠点に帰るときのフラグ
	bool isReturn_ = false;

	//動きを止めるためのフラグ
	bool isStopMove_ = false;

	//拠点から外に出るときのフラグ
	bool isOut_ = false;

	//カメラ俯瞰視点に移行する時のフラグ
	bool isBirdsEye_ = false;

	//死亡フラグ
	bool isDead_ = false;

	//UI関連
	std::unique_ptr<Sprite> lifeLeftGage_;
	std::unique_ptr<Sprite> lifeLeftFrame_;
	std::unique_ptr<Sprite> lifeRightGage_;
	std::unique_ptr<Sprite> lifeRightFrame_;
	std::unique_ptr<Sprite> deadSprite_;

	uint32_t lifeLeftGageTexture_;
	uint32_t lifeLeftFrameTexture_;
	uint32_t lifeRightGageTexture_;
	uint32_t lifeRightFrameTexture_;
	uint32_t deadTexture_;

};


