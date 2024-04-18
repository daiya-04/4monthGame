#pragma once
#include "IScene.h"
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
#include "Engine/Loader/LevelLoader.h"
#include "PostEffect.h"
#include "DualSceneDrawer.h"
#include "Sprite.h"
#include "WeightCircle/WeightCircle.h"
#include "EnvironmentEffects/HeatHaze/HeatHazeManager.h"
#include "EnvironmentEffects/CameraFrozen/CameraFrozenManager.h"
#include "EnvironmentEffects/EnvironmentEffectsManager.h"
#include "EnvironmentEffects/WaterDrop/WaterDropManager.h"
#include "EnvironmentEffects/Snow/SnowManager.h"
class GameScene : public IScene {
public:

	void Init()override;

	void Update()override;

	void DrawNotSetPipeline()override;//�p�C�v���C�����ݒ�(�ŏ��ɌĂяo�����)

	void DrawBackGround()override;

	void DrawObject()override;

	void DrawParticleModel()override;

	void DrawParticle()override;

	void DrawUI()override;

	void DebugGUI()override;

	~GameScene()override;
	
	/// <summary>
	/// �ܔM��Ԃ̃V�[���`��
	/// </summary>
	/// <param name="targetScene">�������ݐ�</param>
	void DrawHeat(PostEffect* targetScene);

	/// <summary>
	/// �Ɋ���Ԃ̃V�[���`��
	/// </summary>
	/// <param name="targetScene">�������ݐ�</param>
	void DrawCold(PostEffect* targetScene);

private:
	ID3D12GraphicsCommandList* commandList_;
	
	HeatHazeManager* heatHazeManager_;
	CameraFrozenManager* cameraFrozen_;
	EnvironmentEffectsManager* environmentEffectsManager_;

	std::unique_ptr<Sprite> sample0;
	std::unique_ptr<Sprite> sample1;

	WaterDropManager* waterDropManager_;
	SnowManager* snowManager_;
	float offset_;
	float roop_;
	float width_;

	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Object2d> testObject_;//雪テスト用
	Vector2 testObjectPosition_ = {640.0f,160.0f};
	Vector3 preCameraPosition_ = {0,0,0};
};

