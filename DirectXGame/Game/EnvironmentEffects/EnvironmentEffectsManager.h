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
	//前シーンのポインタ取得
	PostEffect* GetPrevScene() { return prevScene.get(); };
	//次シーンのポインタ取得
	PostEffect* GetNextScene() { return nextScene.get(); };
	//現在シーンがどちらか(極寒:false 灼熱:true)
	bool GetIsNowScene() { return isNowScene_; };
	//遷移アニメーション再生中か
	bool GetIsPlaySceneChangeAnimation() { return weightCircle_->GetIsStart(); };
	//二重描画用のテクスチャデータ書き込み
	void WeightCircleDraw() { weightCircle_->Draw(); };
	//シーン遷移が完了したフレームか
	bool GetIsChangeComplete() { return isChangeComplete_; };

	//シーンモードを切り替える
	void ChangeSceneMode();
private:
	std::unique_ptr<PostEffect> prevScene;
	std::unique_ptr<PostEffect> nextScene;
	std::unique_ptr<DualSceneDrawer> dualSceneDrawer_;
	std::unique_ptr<WeightCircle> weightCircle_;
	bool isNowScene_;//現在シーンがどちらか(極寒:false 灼熱:true)
	bool isChangeComplete_;//シーン遷移が完了した時にtrue
};
