#pragma once
#include "Block/Block.h"
#include <memory>
#include <array>

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
	void Draw();

	/// <summary>
	/// マップをロードする
	/// </summary>
	/// <param name="stageNumber">ステージ番号</param>
	void Load(uint32_t stageNumber);

	//横の長さ、ブロックの数
	static const uint32_t kMaxStageWidth_ = 200;
	//縦の長さ、ブロックの数
	static const uint32_t kMaxStageHeight_ = 100;

private:

	//マップ
	std::array<std::array<std::unique_ptr<Block>, kMaxStageHeight_>, kMaxStageWidth_> map_;

};


