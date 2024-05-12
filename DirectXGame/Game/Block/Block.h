#pragma once
#include "Object2d.h"
#include "Camera.h"
#include <memory>
#include "Hit.h"
#include "BlockTextureManager.h"
#include "Audio.h"
#include "Score/ScoreManager.h"

class Player;

/// <summary>
/// ブロック全体の基底クラス
/// </summary>
class BaseBlock
{
public:
	
	static const int32_t kBlockSize_ = 96;

	static const int32_t kBlockHalfSize_ = kBlockSize_ / 2;

	const float kTextureBlockSize_ = 32.0f;

	enum BlockType : uint32_t {

		//空白ブロック
		kNone,
		//壊せないブロック
		kUnbreakable = 1,
		//極寒時のブロック
		kSnow,
		//灼熱時のブロック
		kMagma,
		//連続して壊せる氷ブロック
		kIceBlock,
		//速度が速くなるブロック
		kSpeedBlock,
		//採掘速度が上がるブロック
		kDigerBlock,
		//破壊力が上昇するブロック
		kDigPowerBlock,
		//ブロックの最大種類
		kMaxBlock

	};

	//破壊可能ブロックなら、trueを返す関数
	static bool CheckCanBreak(BlockType type) {

		//破壊不可能ブロックなら、falseを返す
		if (type == kUnbreakable) {
			return false;
		}

		return true;

	}

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="type">ブロックの種類</param>
	virtual void Initialize(const Vector2& position, BlockType type) = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw(const Camera& camera) = 0;

	virtual ~BaseBlock() = default;

	/// <summary>
	/// ブロックの種類を変更
	/// </summary>
	/// <param name="type">変更先のブロックの種類</param>
	void ChangeType(BlockType type) { 
		type_ = type;
		texture_ = BlockTextureManager::GetInstance()->GetBlockTexture(type_);
		//object_->SetTextureHandle(texture_);
	}

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	void SetUVPosition(uint32_t xPosition, uint32_t yPosition) { 
		uvPositionX_ = xPosition;
		uvPositionY_ = yPosition;
	}

	//マップチップでの位置を設定
	void SetBlockPosition(uint32_t px, uint32_t py) { blockPositionX_ = px; blockPositionY_ = py; }

	const BlockType& GetType() { return type_; }

	const Vector2& GetPosition() { return position_; }

	//ブロックのマップチップでのX位置取得
	const uint32_t& GetBlockPositionX() { return blockPositionX_; }
	//ブロックのマップチップでのY位置取得
	const uint32_t& GetBlockPositionY() { return blockPositionY_; }
	//当たり判定取得
	const AABB2D& GetCollision() { return collision_; }

	//デバッグ用変数
	bool isDebug_ = true;

	//void SetColor(const Vector4& color) { object_->SetColor(color); }

	//外的要因(プレイヤーなど)で破壊された時に呼び出される関数
	void Break(int32_t power);

	//ブロックが再生する時の関数(再び当たり判定を持つ)
	void Repair() { isBreak_ = false; }

	//ブロックの耐久値設定
	void SetDurability(int32_t value) { 
		defaultDurability_ = value;
		durability_ = defaultDurability_;
	}

	bool GetIsBreak() { return isBreak_; }

	void Reset();

protected:

	ScoreManager* scoreManager_ = nullptr;

	Player* player_;

	BlockType type_ = kUnbreakable;

	//std::unique_ptr<Object2d> object_;

	//当たり判定
	AABB2D collision_{};

	Vector2 position_{};

	uint32_t texture_;

	//uvの座標
	uint32_t uvNumber_ = 0;
	//描画の開始座標を決める数字
	uint32_t uvPositionX_ = 0;
	uint32_t uvPositionY_ = 0;

	//マップチップでのX位置
	uint32_t blockPositionX_ = 0;
	//マップチップでのY位置
	uint32_t blockPositionY_ = 0;

	//破壊されたかどうか
	bool isBreak_ = false;

	//耐久力
	int32_t durability_ = 3;

	int32_t defaultDurability_ = 3;

	//ザクザク音
	Audio* digSE_;

};

class Block : public BaseBlock
{
public:

	Block(const Vector2& position, BlockType type);
	~Block() override;

	void Initialize(const Vector2& position, BlockType type) override;

	void Update() override;

	void Draw(const Camera& camera) override;

private:

};
