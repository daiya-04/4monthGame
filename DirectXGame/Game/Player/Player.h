#pragma once
#include <memory>
#include "Sprite.h"
#include "Vec2.h"

class Player
{
public:
	Player();
	~Player();

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

private:

	std::unique_ptr<Sprite> sprite_;

	//体温の最高値
	const uint32_t kMaxTemperature_ = 140;

	//体温の最低値
	const uint32_t kMinTemperature_ = 80;

	//体温
	uint32_t temperature_ = kMaxTemperature_;

	//水分量の最大値
	const uint32_t kMaxBodyMoisture_ = 100;

	//水分量の最低値
	const uint32_t kMinBodyMoisture_ = 0;

	//水分量
	uint32_t bodyMoisture_ = 50;

	//ジャンプフラグ
	bool canJump_ = true;

	//壁キックフラグ
	bool canWallJump_ = false;

	//ヒップドロップ
	bool canHipDrop_ = false;

	//掴みフラグ
	bool canCatch_ = true;

	//速度
	Vector2 velocity_{};

	//位置
	Vector2 position_{};

};


