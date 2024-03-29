#pragma once
#include "Object2d.h"
#include "Camera.h"
#include <memory>

class Player;

/// <summary>
/// ブロック全体の基底クラス
/// </summary>
class BaseBlock
{
public:
	
	static const uint32_t kBlockSize_ = 32;

	enum BlockType : uint32_t {

		//空白ブロック(使わない)
		kNone,
		//壊せないブロック
		kUnbreakable = 1,
		//極寒時のブロック
		kSnow,
		//サウナストーンブロック
		kSaunaBlock,
		//坂道版ブロック
		kUnbreakableSlope,
		//坂道極寒ブロック
		kSnowSlope,

		//-----エディターで使うブロックここまで-----

		//灼熱時のブロック
		kMagma,
		//坂道灼熱ブロック
		kMagmaSlope,
		//ブロックの最大種類
		kMaxBlock

	};

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
	void ChangeType(BlockType type) { type_ = type; }

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

protected:

	Player* player_;

	BlockType type_ = kUnbreakable;

	std::unique_ptr<Object2d> object_;

	Vector2 position_{};

	uint32_t texture_;

	//uvの座標
	uint32_t uvNumber_;
	//描画の開始座標を決める数字
	uint32_t uvPositionX_ = 0;
	uint32_t uvPositionY_ = 0;

	//マップチップでのX位置
	uint32_t blockPositionX_ = 0;
	//マップチップでのY位置
	uint32_t blockPositionY_ = 0;

};

class Block : public BaseBlock
{
public:

	Block();
	~Block() override;

	void Initialize(const Vector2& position, BlockType type) override;

	void Update() override;

	void Draw(const Camera& camera) override;

private:

};
