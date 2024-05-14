#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
#include <unordered_map>
#include <vector>

#include "Sprite.h"
#include "Object2d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"


class TitleScene : public DefaultScene {
public:

	void Init()override;

	void Update()override;

	void DrawBackGround()override;

	void DrawObject()override;

	void DrawParticle()override;

	void DrawNotSetPipeline()override {};

	void DrawUI()override;

	void DebugGUI()override;

	~TitleScene()override;

	void LoadData();

	void ApplyGlobalVariables();

private:

	const std::string dataName = "TitleUI";

	char filePathBuff[256]{};
	char groupNameBuff[256]{};
	Vector2 addPos{};

	uint32_t bgTexture_;
	std::unique_ptr<Sprite> backGround_;

	std::unordered_map<std::string, std::unique_ptr<Sprite>> uis_;

};
