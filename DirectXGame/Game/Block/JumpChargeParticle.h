#pragma once
#include "Object2d.h"
#include "Camera.h"
#include <memory>
#include "Hit.h"
#include "BlockTextureManager.h"
#include "Audio.h"
#include "Score/ScoreManager.h"

class JumpChargeParticle {
public:

	static int32_t response;

	void Initialize(const Vector2& postition, int32_t type);
	void Update(const Vector2& targetPosition);
	void Draw();
	bool GetIsAlive() { return isAlive_; };
private:
	Vector2 position_{};
	Vector2 offset_{};
	Vector2 velocity_{};
	Vector2 accelerate_{};
	uint32_t type_ = 0;
	//生存時間
	uint32_t aliveTime_ = 0;
	uint32_t aliveMax_ = 0;
	bool isAlive_ = false;
	Vector4 color_;
	float rotate_;
};
