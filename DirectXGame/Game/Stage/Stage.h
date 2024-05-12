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

	//マグマライン取得
	float GetMagmaLine() const { return magmaLine_; }

	//クリアフラグ取得
	bool GetIsClear() const { return isClear_; }

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

	//マグマリセット
	void ResetMagma();

	void SetUV(Block* block);

	//当たり判定関連
	void CheckCollision();

private:

	Player* player_;

	Camera* camera_;

	static const int32_t kMaxBorder_ = 2;

	//岩の数最大桁
	static const int32_t kMaxNumbers_ = 5;

	std::array<std::unique_ptr<Object2d>, kMaxBorder_>  borders_;
	std::unique_ptr<Object2d> magma_;

	//マップ
	static std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth_>, kMaxStageHeight_> map_;

	//マグマのライン
	float maxMagmaLine_ = 12000.0f;
	float magmaLine_ = 12000.0f;
	float magmaTexBaseX_ = 0.0f;
	float magmaUnderLine_ = 10000.0f;

	int32_t rockCount_ = 0;

	//必要な岩の数
	int32_t goalRockCount_ = 100;

	bool isClear_ = false;

	//数字の表示フラグ
	std::array<bool, kMaxNumbers_> isActiveNumber_{};

	//岩の数を表示する数字
	std::array<std::unique_ptr<Sprite>, kMaxNumbers_> numbers_;
	std::unique_ptr<Sprite> clearSprite_;

	//帰還エリア
	std::array<std::unique_ptr<Object2d>, 2> returnObjects_;
	std::array<Vector2, 2> returnPosition_;
	std::array<AABB2D, 2> returnArea_;

	//テクスチャ
	uint32_t numTex_;
	uint32_t clearTex_;
	uint32_t borderTex_;
	uint32_t magmaTex_;
	uint32_t returnTex_;

};


