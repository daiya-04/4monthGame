#pragma once
#include "Sprite.h"
#include <memory>

class Player;

/// <summary>
/// ブロック全体の基底クラス
/// </summary>
class BaseBlock
{
public:
	
	enum BlockType : uint32_t {

		//壊せないブロック
		kUnbreakable,
		//極寒時のブロック
		kSnow,
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
	virtual void Draw() = 0;

	virtual ~BaseBlock() = default;

	/// <summary>
	/// ブロックの種類を変更
	/// </summary>
	/// <param name="type">変更先のブロックの種類</param>
	void ChangeType(BlockType type) { type_ = type; }

	/// <summary>
	/// プレイヤーをセット
	/// </summary>
	void SetPlayer_(std::shared_ptr<Player> player) { player_ = player; }

protected:

	std::shared_ptr<Player> player_;

	BlockType type_ = kUnbreakable;

	std::unique_ptr<Sprite> sprite_;

	Vector2 position_{};

	uint32_t texture_;

};

class Block : public BaseBlock
{
public:

	Block();
	~Block() override;

	void Initialize(const Vector2& position, BlockType type) override;

	void Update() override;

	void Draw() override;

private:

};
