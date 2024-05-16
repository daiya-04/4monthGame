#pragma once
#include "Object2d.h"
#include "Object2dInstancing.h"
#include <memory>
#include "Camera.h"
#include "Block/Block.h"

class Player;

class UpgradeSystem
{
public:
	UpgradeSystem();
	~UpgradeSystem();

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void SetPlayer(Player* player) { player_ = player; }

private:

	Player* player_ = nullptr;

	std::unique_ptr<Object2dInstancing> drawBlocks_;
	
	std::unique_ptr<Object2d> furnace_;

	uint32_t furnaceTexture_;

};


