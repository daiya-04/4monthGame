#pragma once
#include "Block/Block.h"
#include "SaunaStone/SaunaStone.h"
#include <memory>
#include <vector>
#include <array>
#include "Camera.h"
#include <list>

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
	/// マップをロードする
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	void Load(uint32_t stageNumber);

	//横の長さ、ブロックの数
	static const uint32_t kMaxStageWidth_ = 40;
	//縦の長さ、ブロックの数
	static const uint32_t kMaxStageHeight_ = 100;

	//プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }

	//カメラセット
	void SetCamera(Camera* camera) { camera_ = camera; }

	//ブロック取得
	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth_>, kMaxStageHeight_>* GetBlocks() { return &map_; }

	uint32_t blockPositions_[kMaxStageHeight_][kMaxStageWidth_]{};

	static inline const Vector2 kBasePosition = { float(kMaxStageWidth_ * Block::kBlockSize_ / 2.0f - Block::kBlockHalfSize_),
		-80.0f };

	static inline const Vector2 kBorderLeft = { float(kBasePosition.x - 5.0f * Block::kBlockSize_), -75.0f };

	static inline const Vector2 kBorderRight = { float(kBasePosition.x + 5.0f * Block::kBlockSize_), -75.0f };

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

private:

	Player* player_;

	Camera* camera_;

	void SetUV(Block* block);

	//マップ
	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth_>, kMaxStageHeight_> map_;

	//サウナストーン(ステージ毎に数が違っても対応できるようにvector)
	std::vector<std::shared_ptr<SaunaStone>> stones_;

};


