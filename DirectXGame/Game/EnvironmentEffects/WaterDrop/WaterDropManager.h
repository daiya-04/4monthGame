#pragma once
#include "EnvironmentEffects/WaterDrop/WaterDrop.h"
#include "EnvironmentEffects/WaterDrop/WaterDropPipeline.h"
#include "EnvironmentEffects/WaterDrop/WaterDropSceneDrawer.h"

class WaterDropManager
{
public:
	static WaterDropManager* GetInstance() {
		static WaterDropManager instance;
		return &instance;
	}

	void Init();
	void Update();
	//GPUの更新
	void DrawEffectUpdate(PostEffect* sorceTexture);

	void DrawScene(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE prevSceneHandle);

	//アニメーションの初期配置を行う
	void Reset() { waterDrop_->Reset(); };

	//アニメーションを再生する
	void Start() { waterDrop_->Start(); };

private:
	WaterDropManager(){ Init(); };
	~WaterDropManager() = default;

	std::unique_ptr<WaterDrop> waterDrop_;
	std::unique_ptr<WaterDropSceneDrawer> waterDropSceneDrawer_;
};

