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
	//GPUでのエフェクトの更新
	void DrawEffectUpdate(PostEffect* sorceTexture);


	/// <summary>
	/// 水滴エフェクトとシーンを合成して描画
	/// </summary>
	/// /// <param name="targetScene">エフェクトを掛けたいシーンのSrvHandle</param>
	void DrawScene(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE prevSceneHandle);

	//アニメーションの初期配置を行う
	void Reset() { waterDrop_->Reset(); };

	//アニメーションを再生する
	void Start() { waterDrop_->Start(); };

private:
	WaterDropManager(){ Init(); };
	~WaterDropManager() = default;

	std::unique_ptr<WaterDrop> waterDrop_;//水滴エフェクト本体
	std::unique_ptr<WaterDropSceneDrawer> waterDropSceneDrawer_;//水滴エフェクトとシーンを合成する機能
};

