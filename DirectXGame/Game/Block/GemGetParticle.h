#pragma once
#include "Object2d.h"
#include "Camera.h"
#include <memory>
#include "Hit.h"
#include "BlockTextureManager.h"
#include "Audio.h"
#include "Score/ScoreManager.h"
#include <array>
#include "Player/Player.h"
class GemGetParticle {
public:

	enum Phase {
		START,
		MOVE,
		END
	};

	void Initialize(const Vector2& postition, int32_t type);
	void Update(const Camera& camera);
	void Draw();
	bool GetIsAlive() { return isAlive_; };

	static void SetPlayer(Player* player) { player_ = player; };

private:
	Vector2 position_{};
	Vector2 startPosition_{};
	Vector2 velocity_{};
	Vector2 accelerate_{};
	uint32_t blockType_ = 0;
	//生存時間
	uint32_t aliveTime_ = 0;
	uint32_t aliveMax_ = 0;
	bool isAlive_ = false;
	Vector4 color_;

	Phase phase_=START;
	int32_t type_;
	const uint32_t kMoveMax=30;

	static std::array<Vector2, 11> targetPositions_;

	static Player* player_;
};
