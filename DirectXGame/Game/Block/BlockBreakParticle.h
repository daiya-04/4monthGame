#pragma once
#include "Object2d.h"
#include "Camera.h"
#include <memory>
#include "Hit.h"
#include "BlockTextureManager.h"
#include "Audio.h"
#include "Score/ScoreManager.h"

class BlockBreakParticle {
public:
	void Initialize(const Vector2& postition,uint32_t blockType);
	void Initialize(const Vector2& postition, const Vector2& velocity, uint32_t blockType);
	void Update();
	void Draw();
	bool GetIsAlive() { return isAlive_; };
private:
	Vector2 position_{};
	Vector2 velocity_{};
	Vector2 accelerate_{};
	float rotate_;
	Vector2 texBase_;
	uint32_t blockType_=0;
	//生存時間
	uint32_t aliveTime_ = 0;
	bool isAlive_=false;
};
