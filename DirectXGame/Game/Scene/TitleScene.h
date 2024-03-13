#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
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

	void DrawParticleModel()override;

	void DrawParticle()override;

	void DrawUI()override;

	void DebugGUI()override;

	~TitleScene()override;


private:

};
