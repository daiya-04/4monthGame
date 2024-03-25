#pragma once
#include "IScene.h"
#include <memory>
#include <list>
#include <vector>

#include "Sprite.h"
#include "Object3d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "LevelLoader.h"
#include "PostEffect.h"
#include "DualSceneDrawer.h"
#include "Sprite.h"
#include "WeightCircle/WeightCircle.h"
#include "EnvironmentEffects/HeatHaze/HeatHazeManager.h"
#include "EnvironmentEffects/CameraFrozen/CameraFrozenManager.h"
#include "EnvironmentEffects/EnvironmentEffectsManager.h"
class GameScene : public IScene {
public:

	void Init()override;

	void Update()override;

	void DrawNotSetPipeline()override;//パイプライン未設定(最初に呼び出される)

	void DrawBackGround()override;

	void DrawModel()override;

	void DrawParticleModel()override;

	void DrawParticle()override;

	void DrawUI()override;

	void DebugGUI()override;

	~GameScene()override;
	
	/// <summary>
	/// 灼熱状態のシーン描画
	/// </summary>
	/// <param name="targetScene">書き込み先</param>
	void DrawHeat(PostEffect* targetScene);

	/// <summary>
	/// 極寒状態のシーン描画
	/// </summary>
	/// <param name="targetScene">書き込み先</param>
	void DrawCold(PostEffect* targetScene);

private:
	ID3D12GraphicsCommandList* commandList_;
	
	HeatHazeManager* heatHazeManager_;
	CameraFrozenManager* cameraFrozen_;
	EnvironmentEffectsManager* environmentEffectsManager_;

	std::unique_ptr<Sprite> sample0;
	std::unique_ptr<Sprite> sample1;

	//std::unique_ptr<WeightCircle> weightCircle_;

	float offset_;
	float roop_;
	float width_;
};

