#pragma once
#include "Block/Block.h"
#include "SaunaStone/SaunaStone.h"
#include <memory>
#include <vector>
#include <array>
#include "Camera.h"

class Stage
{
public:
	Stage();
	~Stage();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	void Initialize(uint32_t stageNumber);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

	/// <summary>
	/// マップをロードする
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	void Load(uint32_t stageNumber);

	//横の長さ、ブロックの数
	static const uint32_t kMaxStageWidth_ = 200;
	//縦の長さ、ブロックの数
	static const uint32_t kMaxStageHeight_ = 100;

	//プレイヤーをセット
	void SetPlayer(Player* player) { player_ = player; }

private:

	uint32_t blockPositions_[kMaxStageHeight_][kMaxStageWidth_]{};

	Player* player_;

	void SetUV(Block* block);

	//マップ
	/*std::array<std::array<std::shared_ptr<Block>, kMaxStageHeight_>, kMaxStageWidth_> map_;*/
	std::vector<std::shared_ptr<Block>> map_;

	//サウナストーン(ステージ毎に数が違っても対応できるようにvector)
	std::vector<std::shared_ptr<SaunaStone>> stones_;

};


