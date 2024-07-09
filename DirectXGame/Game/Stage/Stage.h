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
#include "System/UpgradeSystem.h"
#include "Magma.h"
#include "EnvironmentEffects/EnvironmentEffectsManager.h"
#include "AudioManager.h"
#include "StageConstant.h"
#include "Player/Player.h"
#include "Enemy/Enemy.h"

class Stage
{
public:
	Stage();
	~Stage();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(uint32_t stageNumber);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//灼熱描画
	void DrawHeatBefore();
	void DrawHeatAfter();
	//極寒描画
	void DrawColdBefore();
	void DrawColdAfter();

	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

	/// <summary>
	/// マップをロードする
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	void Load(uint32_t stageNumber);
	//プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }

	//カメラセット
	void SetCamera(Camera* camera) { camera_ = camera; }

	//マグマゲット
	Magma* GetMagma() { return magma_.get(); }

	//ブロック取得
	std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight>* GetBlocks() { return &map_; }

	uint32_t blockPositions_[kMaxStageHeight][kMaxStageWidth]{};

	static inline const Vector2 kBasePosition =
	{ float(kMaxStageWidth * Block::kBlockSize_ / 2.0f - Block::kBlockHalfSize_),-88.0f };

	static inline const Vector2 kBorderLeft = { float(kBasePosition.x - 5.0f * Block::kBlockSize_), -48.0f };

	static inline const Vector2 kBorderRight = { float(kBasePosition.x + 5.0f * Block::kBlockSize_), -48.0f };

	//マグマライン取得
	float GetMagmaLine() const { return magma_->currentMagmaLine_; }

	//クリアフラグ取得
	bool GetIsClear() const { return isClear_; }

	//スタートフラグセット
	void SetIsStart(bool flag) { isStart_ = flag; }

	//極寒ブロックを灼熱ブロックに変える
	void ChangeSnow2Magma();

	//灼熱ブロックを極寒ブロックに変える
	void ChangeMagma2Snow();

	bool GetIsActiveUpgrade() const { return upgradeSystem_->GetIsActive(); }

	//氷ブロックを溶かす
	void BreakIceBlock();

private:

	//実体生成
	void CreateEntity();

	//ブロック切り替え
	void SwitchBlock();

	//マップの空いてるマスに氷を追加する
	void CreateIceBlock();

	//壊せないブロック以外の破壊(デバッグ)
	void BreakAllBlock();

	//フラグブロックの排除
	void BreakFlagBlock();

	void SetUV(Block* block);

	//当たり判定関連
	void CheckCollision();

	void RespawnBlock(Block::BlockType type);

	void UpdateAnimation();

private:

	EnvironmentEffectsManager* environmentEffectsManager_ = nullptr;

	Player* player_;

	Camera* camera_;

	static const int32_t kMaxBorder_ = 2;

	//岩の数最大桁
	static const int32_t kMaxNumbers_ = 5;

	//現在のステージ番号
	int32_t currentStageNumber_ = 1;

	//チュートリアルステージ用のフラグ
	bool isBreakFlagBlocks_ = false;
	int32_t tutorialSwitchCount_ = 0;
	int32_t maxSwitchCount_ = 240;

	//チュートリアル用のオブジェクト
	std::unique_ptr<Object2d> tutorialFirst_;
	std::unique_ptr<Object2d> tutorialSecond_;
	std::unique_ptr<Object2d> tutorialThird_;

	std::array<std::unique_ptr<Object2d>, kMaxBorder_>  borders_;
	std::unique_ptr<Magma> magma_;
	std::unique_ptr<Object2d> saunaRoom_;
	std::unique_ptr<Object2d> UI_A_;
	std::unique_ptr<Object2d> telescope_;
	std::unique_ptr<Object2d> UI_X_;

	std::array<std::unique_ptr<Object2d>, 2> ropes_;
	std::array<std::unique_ptr<Object2d>, 2> wells_;

	std::unique_ptr<UpgradeSystem> upgradeSystem_;
	bool canUpgrade_ = false;

	//再生成するブロックを纏めたもの
	std::vector<std::array<uint32_t, 2>> respawnBlocks_;

	//マップ
	static std::array<std::array<std::shared_ptr<Block>, kMaxStageWidth>, kMaxStageHeight> map_;

	std::vector<std::shared_ptr<BaseEnemy>> enemies_;

	int32_t rockCount_ = 0;

	//必要な岩の数
	int32_t goalRockCount_ = 100;

	bool isStart_ = false;

	bool isClear_ = false;

	//再生成フラグ
	bool isRespawn_ = false;

	//氷ブロックの破壊開始座標
	int32_t breakStartX_ = 0;
	int32_t breakStartY_ = 0;
	//氷ブロック破壊間隔
	int32_t breakInterval_ = 5;
	int32_t breakTimer_ = 0;

	//数字の表示フラグ
	std::array<bool, kMaxNumbers_> isActiveNumber_{};

	//岩の数を表示する数字
	std::array<std::unique_ptr<Sprite>, kMaxNumbers_> numbers_;
	std::unique_ptr<Sprite> purposeSprite_;

	//帰還エリア
	std::array<Vector2, 2> returnPosition_;
	std::array<AABB2D, 2> returnArea_;
	std::unique_ptr<Object2d> returnUI_;
	bool canReturn_ = false;

	//強化エリア
	Vector2 upgradePosition_{};
	AABB2D upgradeArea_{};

	//テクスチャ
	uint32_t numTex_;
	uint32_t borderBlueTex_;
	uint32_t borderOrangeTex_;
	uint32_t returnTex_;
	uint32_t saunaRoomTex_;
	uint32_t purposeTex_;
	uint32_t upTex_;
	uint32_t ropeTex_;
	uint32_t wellBlueTex_;
	uint32_t wellOrangeTex_;
	uint32_t UI_A_Tex_;
	uint32_t tutorialFirstTex_;
	uint32_t tutorialSecondTex_;
	uint32_t tutorialThirdTex_;
	uint32_t UI_X_Tex_;
	uint32_t telescopeTex_;

///===============アニメーション関連=====================

	//コマ最大数
	int32_t maxFlagAnimationNum_ = 6;
	//現在のコマ
	int32_t currentFlagAnimationNum_ = 0;
	//アニメーション時間
	int32_t flagAnimationTime_ = 0;
	//切り替えフレーム
	int32_t flagChangeFrame_ = 4;

///=============================================


	Audio* meltSE_;
	Audio* upgradeOpenSE_;

};


