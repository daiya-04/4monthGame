#pragma once
#include <memory>
#include "Sprite.h"
#include "Vec2.h"

class SaunaStone
{
public:
	SaunaStone();
	~SaunaStone();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private:

	std::unique_ptr<Sprite> sprite_;

};


