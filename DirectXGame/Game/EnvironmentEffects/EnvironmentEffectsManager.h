#pragma once
#include "DualSceneDrawer.h"
#include "Sprite.h"
#include "WeightCircle/WeightCircle.h"
#include "HeatHaze/HeatHaze.h"
#include "CameraFrozen/CameraFrozenManager.h"
class EnvironmentEffectsManager
{
public:
	static EnvironmentEffectsManager* GetInstance() {
		static EnvironmentEffectsManager instance;
		return &instance;
	}

	EnvironmentEffectsManager();
	~EnvironmentEffectsManager() = default;

	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	PostEffect* GetPrevScene() { return prevScene.get(); };
	PostEffect* GetNextScene() { return nextScene.get(); };
	bool GetIsNowScene() { return isNowScene_; };
	bool GetIsPlaySceneChangeAnimation() { return weightCircle_->GetIsStart(); };
	void WeightCircleDraw() { weightCircle_->Draw(); };
private:
	std::unique_ptr<PostEffect> prevScene;
	std::unique_ptr<PostEffect> nextScene;
	std::unique_ptr<DualSceneDrawer> dualSceneDrawer_;
	std::unique_ptr<WeightCircle> weightCircle_;
	bool isNowScene_;//現在シーンがどちらか(極寒:false 灼熱:true)
};
