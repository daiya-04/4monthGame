#pragma once
#include "Block/Block.h"
#include "SaunaStone/SaunaStone.h"
#include <memory>
#include <vector>
#include <array>
#include "Camera.h"
#include <list>
#include "Object2d.h"
#include "Sprite.h"

class Player;

class Stage
{
public:
	Stage();
	~Stage();

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
	void Draw();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// マップをロードする
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	void Load(uint32_t stageNumber);

	//横の長さ、ブロックの数
	static const int32_t kMaxStageWidth_ = 40;
	//縦の長さ、ブロックの数
	static const int32_t kMaxStageHeight_ = 100;

	//プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }

	//カメラセット
	void SetCamera(Camera* camera) { camera_ = camera; }

	//ブロック取得
	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth_>, kMaxStageHeight_>* GetBlocks() { return &map_; }

	uint32_t blockPositions_[kMaxStageHeight_][kMaxStageWidth_]{};

	static inline const Vector2 kBasePosition =
	{ float(kMaxStageWidth_ * Block::kBlockSize_ / 2.0f - Block::kBlockHalfSize_),-80.0f };

	static inline const Vector2 kBorderLeft = { float(kBasePosition.x - 5.0f * Block::kBlockSize_), -48.0f };

	static inline const Vector2 kBorderRight = { float(kBasePosition.x + 5.0f * Block::kBlockSize_), -48.0f };

private:

	//実体生成
	void CreateEntity();

	//ブロック切り替え
	void SwitchBlock();

	//マップの空いてるマスに氷を追加する
	void CreateIceBlock();

	//氷ブロックを溶かす
	void BreakIceBlock();

	//壊せないブロック以外の破壊(デバッグ)
	void BreakAllBlock();

	void SetUV(Block* block);

private:

	Player* player_;

	Camera* camera_;

	static const int32_t kMaxNumber_ = 2;

	std::array<std::unique_ptr<Object2d>, kMaxNumber_>  borders_;
	std::unique_ptr<Object2d> magma_;

	//マップ
	static std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth_>, kMaxStageHeight_> map_;

	//マグマのライン
	float magmaLine_ = 5000.0f;
	float magmaTexBaseX_ = 0.0f;

	//パーツの数
	int32_t remainingParts_ = 0;

	bool isClear_ = false;

	//パーツ数を表示する数字
	std::array<std::unique_ptr<Sprite>, 2> numbers_;
	std::unique_ptr<Sprite> clearSprite_;

	//テクスチャ
	uint32_t numTex_;
	uint32_t clearTex_;
	uint32_t borderTex_;
	uint32_t magmaTex_;

};

