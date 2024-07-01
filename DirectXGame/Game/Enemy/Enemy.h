#pragma once
#include "Object2d.h"
#include "Vec2.h"
#include "Camera.h"
#include "TextureManager.h"
#include "AudioManager.h"
#include <memory>
#include "Hit.h"

class BaseEnemy
{
public:

	BaseEnemy();
	~BaseEnemy() = default;

	virtual void Initialize(const Vector2& position) = 0;

	virtual void Update() = 0;

	virtual void Draw(const Camera& camera) = 0;

protected:

	Vector2 velocity_{};

	Vector2 position_{};

	uint32_t enemyHalfSize_ = 32;

	std::unique_ptr<Object2d> object_;

	AABB2D collision_{};

	uint32_t enemyTexture_;

};

class NormalEnemy : public BaseEnemy 
{
public:

	void Initialize(const Vector2& position) override;

	void Update() override;

	void Draw(const Camera& camera) override;

private:


};
