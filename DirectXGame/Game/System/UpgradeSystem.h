#pragma once
#include "Object2d.h"
#include "Object2dInstancing.h"
#include <memory>
#include "Camera.h"
#include "Block/Block.h"
#include "Input.h"
#include <array>

class Player;

class UpgradeSystem
{
public:
	UpgradeSystem();
	~UpgradeSystem();

	enum SelectType {
		kSendRock, //岩送り
		kUpgrade, //強化
		kReturn, //遊びに戻る

		kMaxSelectType,

	};

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void SetPlayer(Player* player) { player_ = player; }

	void SetIsActive(bool flag) { isActive_ = flag; }

private:

	void SendRock();

	void Upgrade();

private:

	Input* input_ = nullptr;

	Player* player_ = nullptr;

	//選択画面を開いているか
	bool preIsActive_ = false;
	bool isActive_ = false;

	//強化系の画面を開いているか
	bool isActiveUpgrade_ = false;

	//選択画面でどれを選んでいるか
	SelectType type_ = kSendRock;

	//強化画面でどれを選んでいるか
	SelectType upgradeType_ = kSendRock;

	std::unique_ptr<Object2dInstancing> drawBlocks_;
	
	std::unique_ptr<Object2d> furnace_;

	uint32_t furnaceTexture_;

};


