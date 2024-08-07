#pragma once
#include <memory>
#include "BlockTextureManager.h"

class HitEffect {
public:
	void Initialize(const Vector2& postition,int32_t type);
	void Update();
	void Draw();
	bool GetIsAlive() { return isAlive_; };
private:
	Vector2 position_{};
	Vector2 size_{};
	//生存時間
	uint32_t aliveTime_ = 0;
	uint32_t aliveMax_ = 0;
	bool isAlive_ = false;
	Vector4 color_;
	float rotate_;
};
