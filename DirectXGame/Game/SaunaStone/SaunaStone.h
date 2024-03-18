#pragma once
#include <memory>
#include "Object2d.h"
#include "Vec2.h"
#include "Camera.h"

class SaunaStone
{
public:
	SaunaStone();
	~SaunaStone();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector2& position);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(const Camera& camera);

	const Vector2& GetPosition() { return position_; }

private:

	std::unique_ptr<Object2d> object_;

	uint32_t texture_;

	Vector2 position_{};

};


