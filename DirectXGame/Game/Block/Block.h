#pragma once
#include "Object2d.h"
#include "Camera.h"
#include <memory>
#include "Hit.h"
#include "BlockTextureManager.h"
#include "Audio.h"
#include "Score/ScoreManager.h"

class Player;

class Magma;

/// <summary>
/// ブロック全体の基底クラス
/// </summary>
class BaseBlock
{
public:
	
	static const int32_t kBlockSize_ = 96;

	static const int32_t kBlockHalfSize_ = kBlockSize_ / 2;

	const float kTextureBlockSize_ = 32.0f;

	//光る確率
	static inline const float kShiningProbability_ = 0.0006f;

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
		kRedBlock,
		//採掘速度が上がるブロック
		kGreenBlock,
		//破壊力が上昇するブロック
		kBlueBlock,
		//マグマを下降させるブロック
		kDownMagma,
		//黄金ブロック(クリア条件)
		kGoldBlock,
		//チュートリアル時に使う特殊なブロック
		kFlagBlock,
		//崩れるブロック
		kCollapseBlock,
		//敵追加ブロック
		kEnemyBlock,
		//トゲブロック(敵と同じ扱いをする)
		kNeedleBlock,
		//

		//ブロックの最大種類
		kMaxBlock

	};

	enum IceBreakDirection {
		kUp,
		kDown,
		kLeft,
		kRight,
	};

	//破壊可能ブロックなら、trueを返す関数
	static bool CheckCanBreak(BlockType type) {

		//破壊不可能ブロックなら、falseを返す
		if (type == kUnbreakable || type == kFlagBlock) {
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
		defaultType_ = type;
		type_ = type;
		//object_->SetTextureHandle(texture_);
	}

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	void SetPlayer(Player* player) { player_ = player; }

	void SetMagma(Magma* magma) { magma_ = magma; }

	static void SetScore(Score* score) { score_ = score; }

	void SetUVPosition(uint32_t xPosition, uint32_t yPosition) { 
		uvPositionX_ = xPosition;
		uvPositionY_ = yPosition;
	}

	//マップチップでの位置を設定
	void SetBlockPosition(uint32_t px, uint32_t py) { blockPositionX_ = px; blockPositionY_ = py; }

	const BlockType& GetType() { return type_; }

	const BlockType& GetDefaultType() { return defaultType_; }

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
	void Break(float power, bool isPlayer = true);

	//破壊できないブロックに対して攻撃が行われたときに呼ぶ
	void UnBreak();

	//ブロックが再生する時の関数(再び当たり判定を持つ)
	void Repair() { 
		durability_ = defaultDurability_;
		isBreak_ = false;
		isStartBreak_ = false;
		iceBreakCoolTimer_ = 0;
	}

	//ブロックの耐久値設定
	void SetDurability(float value) { 
		defaultDurability_ = value;
		durability_ = defaultDurability_;
		SetColor();
	}

	//ブロックの崩壊フラグ設定
	void SetCollapse();

	bool GetIsStartCollapse() const { return isStartCollapse_; }

	void SetColor();

	bool GetIsBreak() { return isBreak_; }

	void Reset();

	void IceBreak(int32_t breakCoolTime = 0, bool scoreFlag = true);

	//氷ブロックを溶かす処理(スコア加算無し)
	void MeltIce();

	//上下左右隣接しているポインタ取得
	void SetPointer(BaseBlock* left, BaseBlock* right, BaseBlock* up, BaseBlock* down) {
		pLeft_ = left;
		pRight_ = right;
		pUp_ = up;
		pDown_ = down;
	}

	void SetDirection(IceBreakDirection direction) { direction_ = direction; }

protected:

	static Score* score_;

	Player* player_;

	Magma* magma_ = nullptr;

	BaseBlock* pLeft_ = nullptr;
	BaseBlock* pRight_ = nullptr;
	BaseBlock* pUp_ = nullptr;
	BaseBlock* pDown_ = nullptr;

	IceBreakDirection direction_ = kUp;

	BlockType type_ = kUnbreakable;

	BlockType defaultType_ = kUnbreakable;

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

	//スコア加算するか
	bool isAddScore_ = true;

	//崩壊フラグ
	bool isStartCollapse_ = false;
	//崩壊カウント
	int32_t maxCollapseCount_ = 40;
	int32_t collapseCount_ = maxCollapseCount_;

	//耐久力
	float durability_ = 3.0f;

	float defaultDurability_ = 3.0f;

	///-----氷専用-------------------------------------------------------------

	bool isStartBreak_ = false;

	int32_t iceBreakCoolTimer_ = 0;

	int32_t iceBreakInterval_ = 5;

	///-------------------------------------------------------------------------

	///--------演出関連--------------------------------------------------------

	bool isShining_ = false;

	//光の値
	float brightValue_ = 0.0f;

	//光制限
	float brightLimit_ = 1.0f;

	float shiningValue_ = 0.02f;

	///-------------------------------------------------------------------------

	//ザクザク音
	Audio* digLowSE_;
	Audio* digMidSE_;
	Audio* digHighSE_;
	Audio* crystalSE_;
	Audio* collapseSE_;
	Audio* ice1SE_;
	Audio* ice2SE_;
	Audio* ice3SE_;

	Vector4 color_{ 1.0f,1.0f,1.0f,1.0f };

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
