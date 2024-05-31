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

	void SetColor(Vector4 color) { magma_->SetColor(color); }

	//マグマのライン
	float maxMagmaLine_ = Block::kBlockSize_ * 6.0f;
	//基準のライン
	float baseMagmaLine_ = Block::kBlockSize_ * 6.0f;
	//描画で適用するライン
	float currentMagmaLine_ = Block::kBlockSize_ * 6.0f;
	float magmaTexBaseX_ = 0.0f;
	float magmaUnderLine_ = 10000.0f;
	//マグマ制限
	float magmaLimit_ = Block::kBlockSize_ * 6.0f;
	//マグマ速度
	float magmaSpeed_ = 1.0f;
	float magmaSpeedFaster_ = 1.5f;
	//硬直時間
	int32_t freezeTime_ = 0;
	//イージング変数
	float easingT_ = 0.0f;

private:

	void ResetMagma();

	Player* player_ = nullptr;

	std::unique_ptr<Object2d> magma_;

	uint32_t magmaTex_;

};


