#pragma once
#include "DualSceneDrawer.h"
#include "Sprite.h"
#include "WeightCircle/WeightCircle.h"
#include "HeatHaze/HeatHaze.h"
#include "CameraFrozen/CameraFrozenManager.h"
#include "Camera.h"
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

	//まぐま下げるブロックが壊れた時に呼ぶ
	void MagmaDown(const Vector2& center) {
		if (!weightCircle_->GetIsStart()) {
			magmaCenter_ = center;
			//weightCircle_->SetPosition(center - Vector2{camera_->translation_.x, camera_->translation_.y});
			weightCircle_->SetPosition(magmaCenter_ - Vector2{ camera_->translation_.x + camera_->center_.x - 640.0f,camera_->translation_.y + camera_->center_.y - 360.0f });
			weightCircle_->Start();
			isPlayDownMagmaEffect_ = true;
			downMagmaEffectInterval_ = kDownMagmaEffectInterval;
			isCompletefirstChange_ = false;
		}
	};

	void SetCamera(Camera* camera) { camera_ = camera; };

	//灼熱モードで始めるか
	void InitializeStage(bool isHeat) {
		weightCircle_->Reset();
		isNowScene_ = isHeat;
		isChangeComplete_ = false;
		isPlayDownMagmaEffect_ = false;
	}


	bool GetIsPlayDownMagma() {return isPlayDownMagmaEffect_;};

private:
	std::unique_ptr<PostEffect> prevScene;
	std::unique_ptr<PostEffect> nextScene;
	std::unique_ptr<DualSceneDrawer> dualSceneDrawer_;
	std::unique_ptr<WeightCircle> weightCircle_;
	bool isNowScene_;//現在シーンがどちらか(極寒:false 灼熱:true)
	bool isChangeComplete_;//シーン遷移が完了した時にtrue

	bool isPlayDownMagmaEffect_;//まぐまが下がったときに2回切り替えエフェクトを呼ぶ
	bool isCompletefirstChange_;//一回目の遷移が完了したか
	const int32_t kDownMagmaEffectInterval=1;
	int32_t downMagmaEffectInterval_;//中間シーンの長さ
	Camera* camera_=nullptr;
	Vector2 magmaCenter_;
};
