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
#include "Stage/StageConstant.h"
#include <list>
#include "Sprite.h"
#include "Score/ScoreManager.h"
#include "AudioManager.h"
#include "Block/Block.h"

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

	/// <summary>
	/// 移動方法。通常移動と直線高速移動
	/// </summary>
	enum MoveType {
		kNormal, //通常移動
		kLine, //直線移動
	};

	

	//現在操作しているプレイヤーが保有しているブロックの構造体
	struct BringRocks {

		enum RockType {
			kRock, //通常の岩
			kRed, //移動速度
			kGreen, //採掘速度
			kBlue, //ダメージ量
			kMaxType, //種類
		};

		std::array<int32_t, RockType::kMaxType> rocks_{};

	};

	//ブロック1つ当たりの加算量
	struct AddValue {
		//スピード
		float speed = 0.5f;
		//採掘速度
		int32_t digSpeed = 1;
		//採掘のダメージ量
		int32_t digPower = 1;
	};

	//プレイヤーのサイズ(当たり判定)

	//Xの大きさ
	static const uint32_t kPlayerSizeX_ = 48;
	//Yの大きさ
	static const uint32_t kPlayerSizeY_ = 80;
	//プレイヤーサイズの半分X
	static const uint32_t kPlayerHalfSizeX_ = kPlayerSizeX_ / 2;
	//プレイヤーサイズの半分Y
	static const uint32_t kPlayerHalfSizeY_ = kPlayerSizeY_ / 2;
	//画像上のプレイヤーサイズ
	static const uint32_t kPlayerImageSize_ = 96;

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	
	/// <summary>
	/// UI更新
	/// </summary>
	void UpdateUI();

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

	//個々での四角形当たり判定
	void CheckCollisionSquare(const AABB2D& collision);

	void SetPosition(const Vector2& position) { 
		position_ = position;
		//4頂点の座標を更新
		leftTop_ = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
		rightTop_ = { position_.x + kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
		leftBottom_ = { position_.x - kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ };
		rightBottom_ = { position_.x + kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ };
		//当たり判定更新
		collision_.min = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
		collision_.max = { position_.x + kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ };

	}

	void SetTmpPosition(const Vector2& position) {
		tmpPosition_ = position;
		//4頂点の座標を更新
		leftTop_ = { tmpPosition_.x - kPlayerHalfSizeX_, tmpPosition_.y - kPlayerHalfSizeY_ };
		rightTop_ = { tmpPosition_.x + kPlayerHalfSizeX_ - 1, tmpPosition_.y - kPlayerHalfSizeY_ };
		leftBottom_ = { tmpPosition_.x - kPlayerHalfSizeX_, tmpPosition_.y + kPlayerHalfSizeY_ - 1 };
		rightBottom_ = { tmpPosition_.x + kPlayerHalfSizeX_ - 1, tmpPosition_.y + kPlayerHalfSizeY_ - 1 };
		//当たり判定更新
		collision_.min = { tmpPosition_.x - kPlayerHalfSizeX_, tmpPosition_.y - kPlayerHalfSizeY_ };
		collision_.max = { tmpPosition_.x + kPlayerHalfSizeX_ - 1, tmpPosition_.y + kPlayerHalfSizeY_ - 1 };

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

	void SetVelocityY(float value) { velocity_.y = value; }

	void SetBlocks(std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* blocks) { blocksPtr_ = blocks; }

	bool GetIsDead() const { return isDead_; }

	bool GetIsBirdsEye() const { return isBirdsEye_; }

	void SetIsBirdsEye(bool flag) { isBirdsEye_ = flag; }

	bool GetIsStartFade() const { return isStartFade_; }

	void SetIsStartFade(bool flag) { isStartFade_ = flag; }

	bool GetIsHome() const { return isHome_; }

	bool GetIsMine() const { return isMining_; }

	bool GetCanJump() const { return parameters_[currentCharacters_]->Jump_.canJump; }

	//クリアフラグのセット
	void SetIsClear(bool flag) { isClear_ = flag; }

	bool GetIsClear() const { return isClear_; }

	//岩の受け渡し
	void SendRocks(int32_t& rockCount) {
		rockCount += rockParameter_.rocks_[BringRocks::kRock];
		rockParameter_.rocks_[BringRocks::kRock] = 0;
	}

	//スピードの岩を加算
	void AddBlueRock(int32_t addNum = 1) { 
		rockParameter_.rocks_[BringRocks::kBlue] += addNum;
		rockParameter_.rocks_[BringRocks::kBlue] = std::clamp(rockParameter_.rocks_[BringRocks::kBlue], 0, 999);
	}

	//採掘速度の岩を加算
	void AddGreenRock(int32_t addNum = 1){ 
		rockParameter_.rocks_[BringRocks::kGreen] += addNum;
		rockParameter_.rocks_[BringRocks::kGreen] = std::clamp(rockParameter_.rocks_[BringRocks::kGreen], 0, 999);
	}

	//採掘ダメージ量の岩を加算
	void AddRedRock(int32_t addNum = 1) { 
		rockParameter_.rocks_[BringRocks::kRed] += addNum;
		rockParameter_.rocks_[BringRocks::kRed] = std::clamp(rockParameter_.rocks_[BringRocks::kRed], 0, 999);
	}

	//岩カウント加算
	void AddRockCount(int32_t addNum = 1) { 
		rockParameter_.rocks_[BringRocks::kRock] += addNum;
		rockParameter_.rocks_[BringRocks::kRock] = std::clamp(rockParameter_.rocks_[BringRocks::kRock], 0, 999);
	}

	//速度強化
	void UpgradeSpeed(float addNum) { 

		parameters_[currentCharacters_]->speed_ += addNum;
		parameters_[currentCharacters_]->maxMoveSpeed_ += addNum * 1.0f;

		parameters_[currentCharacters_]->maxMoveSpeed_ = std::clamp(parameters_[currentCharacters_]->maxMoveSpeed_, 0.0f, 30.0f);
		parameters_[currentCharacters_]->maxDefaultMoveSpeed_ = parameters_[currentCharacters_]->maxMoveSpeed_;
		parameters_[currentCharacters_]->maxChargeMoveSpeed_ = parameters_[currentCharacters_]->maxMoveSpeed_ / 3.0f;

	}

	//採掘速度強化
	void UpgradeDigSpeed(int32_t addNum) { 
		parameters_[currentCharacters_]->dig_.digInterval -= addNum;
		parameters_[currentCharacters_]->dig_.digInterval = std::clamp(parameters_[currentCharacters_]->dig_.digInterval, 1, 60);
	}

	//パワーーー！！！を強化
	void UpgradePower(float addNum) { parameters_[currentCharacters_]->dig_.digPower += addNum; }

	//ブロックの数取得
	int32_t GetRockCount() const { return rockParameter_.rocks_[BringRocks::kRock]; }

	//リフト関連処理
	void MoveLift();

	BringRocks& GetRockParameter() { return rockParameter_; }

	Characters GetCurrentCharacter() const { return currentCharacters_; }

	//死亡処理
	void Restart();

	void HealUpdate();

	//溜めジャンプ制限
	void RestrictChargeJump() { parameters_[currentCharacters_]->chargeJump_.chargeTimer = 0; }

	//穴掘りカウント
	void AddDigCount(int32_t val = 1) { digCount_ += val; }

	int32_t GetDigCount() const { return digCount_; }

	void ResetDigCount() { digCount_ = 0; }

	void Stun();

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

	//jsonファイルから読み取ったパラメータを適用
	void LoadParameter();

private:

	Input* input_;

	ScoreManager* scoreManager_ = nullptr;

	std::unique_ptr<Object2d> object_;

	//ブロックの配列ポインタ
	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* blocksPtr_ = nullptr;

	//当たり判定
	AABB2D collision_{};

	//デフォルトパラメータ。調整、初期化用
	std::unique_ptr<PlayerParameter> defaultParameter_;

	//パラメータを纏めたもの
	std::array<std::unique_ptr<PlayerParameter>, kMaxPlayer> parameters_;

	//パラメータ値加算量を蓄積するもの
	/*std::array<BringRocks, kMaxPlayer> rockParameters_;*/
	//共有に変更
	BringRocks rockParameter_;

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

	//死んだ場所
	Vector2 deadPosition_{};
	//再開地点
	Vector2 restartPosition_{};

	//プレイヤー画像
	std::array<uint32_t, kMaxPlayer> texture_;
	std::array<uint32_t, kMaxPlayer> textureUp_;
	std::array<uint32_t, kMaxPlayer> textureDown_;
	std::array<uint32_t, kMaxPlayer> textureRun_;
	std::array<uint32_t, kMaxPlayer> textureBreakRun_;
	std::array<uint32_t, kMaxPlayer> textureBreakUp_;
	std::array<uint32_t, kMaxPlayer> textureBreakDown_;
	std::array<uint32_t, kMaxPlayer> textureBreak_;
	std::array<uint32_t, kMaxPlayer> textureWallJump_;

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

	//拠点にいる時のフラグ
	bool isHome_ = true;

	//カメラ俯瞰視点に移行する時のフラグ
	bool isBirdsEye_ = false;

	//フェード開始フラグ
	bool isStartFade_ = false;

	//画像を上に上げるフラグ
	bool isUpImage_ = false;

	//スタンタイマー
	int32_t stunTimer_ = 0;
	//無敵時間
	int32_t invincibleTimer_ = 0;

	//死亡フラグ
	bool isDead_ = false;
	//再スタートまでのタイマー
	int32_t respwanTimer_ = 0;
	//再スタートにかかる時間
	int32_t respawnCoolTime_ = 60;

	//クリアフラグ
	bool isClear_ = false;

	//移動タイプ
	MoveType moveType_ = kNormal;

	//ダメージ間隔
	int32_t damageTimer_ = 120;

	//コマ最大数
	int32_t maxAnimationNum_ = 8;
	//現在のコマ
	int32_t currentAnimationNum_ = 0;

	//アニメーション時間
	int32_t animationTime_ = 0;

	//切り替えフレーム
	int32_t changeFrame_ = 4;

	//最大桁数
	static const int32_t kMaxDigits_ = 3;

	//UI関連
	std::array<std::array<std::unique_ptr<Sprite>, kMaxDigits_>, BringRocks::RockType::kMaxType> numbers_;
	std::array<std::unique_ptr<Sprite>, BringRocks::RockType::kMaxType> rocksUI_;
	std::unique_ptr<Sprite> bag_;

	uint32_t numberTexture_;
	uint32_t bagTexture_;

	const std::string dataName = "Player";

	int32_t digCount_ = 0;

	//歩きのSE制御
	int32_t playSEInterval_ = 15;

	int32_t playSETimer_ = 0;

	///-----SE----------------------------------------

	Audio* walk1SE_;
	Audio* walk2SE_;
	Audio* walk3SE_;
	Audio* walk4SE_;
	Audio* deadSE_;
	Audio* wallJumpSE_;
	Audio* doorCloseSE_;
	Audio* doorOpenSE_;
	Audio* jumpSE_;
	Audio* chargeJumpSE_;

	///------------------------------------------------

};


