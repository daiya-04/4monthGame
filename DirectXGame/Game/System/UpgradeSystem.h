#pragma once
#include "Object2d.h"
#include "Object2dInstancing.h"
#include <memory>
#include "Camera.h"
#include "Block/Block.h"
#include "Input.h"
#include <array>
#include <queue>

class Player;

class UpgradeSystem
{
public:
	UpgradeSystem();
	~UpgradeSystem();

	enum SelectType : int32_t {
		kPower, //採掘破壊力
		kDigSpeed, //採掘速度
		kSpeed, //移動速度

		kMaxSelectType,

	};

	void Initialize(int32_t stageNum);

	void Update();

	void Draw(const Camera& camera);

	void DrawUI();

	void SetPlayer(Player* player) { player_ = player; }

	void SetIsActive(bool flag) { isActive_ = flag; }

	bool GetIsActive() const { return isActive_; }

	bool GetPreIsActive() const { return preIsActive_; }

	void SetGoalCount(int32_t* count) { goalCount_ = count; }

private:

	void SendRock();

	void Upgrade();

	void CheckCanUpgrade(SelectType type);

	void LoadData(int32_t stageNum);

private:

	Input* input_ = nullptr;

	Player* player_ = nullptr;

	//ステージのゴールカウントのポインタ
	int32_t* goalCount_ = nullptr;

	//選択画面を開いているか
	bool preIsActive_ = false;
	bool isActive_ = false;

	//強化系の画面を開いているか
	bool isActiveUpgrade_ = false;

	//選択画面でどれを選んでいるか
	SelectType type_ = kPower;

	//強化画面でどれを選んでいるか
	SelectType upgradeType_ = kPower;

	//スピード強化量
	float speedUpgradeValue_ = 0.2f;

	//採掘速度強化量
	int32_t digSpeedUpgradeValue_ = 1;

	//パワー強化量
	float powerUpgradeValue_ = 0.5f;

	//ブロック座標
	std::vector<Vector2> positions_;
	
	//最大レベル
	static const int32_t kMaxLevel_ = 10;

	//強化に必要な岩数
	std::array<std::array<int32_t, 4>, kMaxLevel_> speedUpgradeNeeds_;
	std::array<std::array<int32_t, 4>, kMaxLevel_> powerUpgradeNeeds_;
	std::array<std::array<int32_t, 4>, kMaxLevel_> digSpeedUpgradeNeeds_;

	//強化レベル
	std::array<int32_t, 2> speedLevel_ = { 0,0 };

	std::array<int32_t, 2> powerLevel_ = { 0,0 };

	std::array<int32_t, 2> digSpeedLevel_ = { 0,0 };

	//有効桁
	static const int32_t kMaxDigits_ = 3;

	//--------------UI関連---------------------

	std::unique_ptr<Object2dInstancing> sendRocks_;
	
	std::unique_ptr<Object2d> furnace_;

	std::unique_ptr<Sprite> upgradeSprite_;

	std::array<std::array<std::unique_ptr<Sprite>, kMaxDigits_>, 4> numbers_;

	std::array<std::unique_ptr<Sprite>, 4> rocksUI_;

	uint32_t furnaceTexture_;
	uint32_t upgradeTexture_;
	uint32_t numberTexture_;
	std::array<uint32_t, 4> rockUITextures_;

	Vector2 UITopPosition_{ 1000.0f,200.0f };
	Vector2 UIMiddlePosition_{ 1000.0f,400.0f };
	Vector2 UIBottomPosition_{ 1000.0f,600.0f };
	std::array<Vector2, 4> numbersPosition_;

};


