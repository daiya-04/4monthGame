#pragma once
#include "Block/Block.h"
#include "Object2d.h"
#include <memory>
#include "TextureManager.h"
#include "Camera.h"

class Player;

class Magma
{
public:
	Magma();
	~Magma();

	void Initialize();

	void Update();

	void Draw(const Camera& camera);

	void SetPlayer(Player* player) { player_ = player; }

	//マグマのライン
	float maxMagmaLine_ = 5000.0f;
	//基準のライン
	float baseMagmaLine_ = 5000.0f;
	//描画で適用するライン
	float currentMagmaLine_ = 5000.0f;
	float magmaTexBaseX_ = 0.0f;
	float magmaUnderLine_ = 10000.0f;
	//マグマ制限
	float magmaLimit_ = Block::kBlockSize_ * 0.0f;
	//マグマ速度
	float magmaSpeed_ = 15.5f;

private:

	void ResetMagma();

	Player* player_ = nullptr;

	std::unique_ptr<Object2d> magma_;

	uint32_t magmaTex_;

};


