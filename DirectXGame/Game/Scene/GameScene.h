#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
#include <vector>
#include <array>

#include "Sprite.h"
#include "Object2d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Stage/Stage.h"
#include "Player/Player.h"
#include "Scroll/Scroll.h"

class GameScene : public DefaultScene {
public:

	void Init()override;

	void Update()override;

	void DrawBackGround()override;

	void DrawObject()override;

	void DrawParticle()override;

	void DrawUI()override;

	void DebugGUI()override;

	~GameScene()override;
	

private:

	void Reset();

	ScoreManager* scoreManager_ = nullptr;

	std::shared_ptr<Camera> camera_;

	std::unique_ptr<Scroll> scroll_;

	Vector2 scorePosition_ = { 256.0f,64.0f };

	//UI関連
	std::unique_ptr<Sprite> backGround_;
	std::unique_ptr<Sprite> pose_;
	std::unique_ptr<Sprite> black_;

	uint32_t bgTexture_;
	uint32_t poseTex_;

};

