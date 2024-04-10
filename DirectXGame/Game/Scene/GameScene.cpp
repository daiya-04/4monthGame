#include "GameScene.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>

GameScene::~GameScene() {}

void GameScene::Init(){
	commandList_ = DirectXCommon::GetInstance()->GetCommandList();
	
	environmentEffectsManager_ = EnvironmentEffectsManager::GetInstance();
	cameraFrozen_ = CameraFrozenManager::GetInstance();
	heatHazeManager_ = HeatHazeManager::GetInstance();

	sample0.reset(new Sprite(TextureManager::GetInstance()->Load("sample0.png"), {640.0f,360.0f},10.0f));
	sample0->Initialize();
	sample1.reset(new Sprite(TextureManager::GetInstance()->Load("hhTest.png"), { 640.0f,360.0f }, 1.0f));
	sample1->Initialize();
	
	waterDropManager_ = WaterDropManager::GetInstance();
}

void GameScene::Update(){
	DebugGUI();
	environmentEffectsManager_->Update();
	heatHazeManager_->Update();
	waterDropManager_->Update();
	//遷移完了時にエフェクトを発行する
	if (environmentEffectsManager_->GetIsChangeComplete()) {
		if (!environmentEffectsManager_->GetIsNowScene()) {
			cameraFrozen_->Start();
			//waterDropManager_->Reset();
		}
		else {
			cameraFrozen_->Reset();
			waterDropManager_->Reset();
			waterDropManager_->Start();
		}
	}
	cameraFrozen_->Update();
}

void GameScene::DrawNotSetPipeline() {
	cameraFrozen_->DrawInternal(commandList_);
	waterDropManager_->DrawEffectUpdate(cameraFrozen_->GetEffectTexture());
	//極寒状態だったら
	if (!environmentEffectsManager_->GetIsNowScene()) {
		DrawCold(environmentEffectsManager_->GetPrevScene());
		//切り替えアニメーション中は裏に反対の状態を描画する
		if (environmentEffectsManager_->GetIsPlaySceneChangeAnimation()) {
			DrawHeat(environmentEffectsManager_->GetNextScene());
		}
	}
	else {
		DrawHeat(environmentEffectsManager_->GetPrevScene());
		if (environmentEffectsManager_->GetIsPlaySceneChangeAnimation()) {
			DrawCold(environmentEffectsManager_->GetNextScene());
		}
	}
	
	//二重描画用のテクスチャデータ書き込み
	if (environmentEffectsManager_->GetIsPlaySceneChangeAnimation()) {
		environmentEffectsManager_->WeightCircleDraw();
	}

}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawObject(){

	

}

void GameScene::DrawParticleModel(){



}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){
	
	//dualSceneDrawer_->Draw(commandList_, prevScene->GetSrvHandleGPU(), nextScene->GetSrvHandleGPU(), weightCircle_->GetHandle());
	environmentEffectsManager_->Draw(commandList_);
}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	

#endif // _DEBUG
}

void GameScene::DrawCold(PostEffect* targetScene) {
	//エフェクトの描画
	//cameraFrozen_->DrawInternal(commandList_);
	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	Sprite::preDraw(commandList_);
	sample0->Draw();
	Sprite::postDraw();
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);


}

void GameScene::DrawHeat(PostEffect* targetScene) {
	//cameraFrozen_->DrawInternal(commandList_);
	heatHazeManager_->PreDraw(commandList_);
	Sprite::preDraw(commandList_);
	sample1->Draw();
	Sprite::postDraw();
	heatHazeManager_->PostDraw(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	//heatHazeManager_->Draw(commandList_);
	waterDropManager_->DrawScene(commandList_, heatHazeManager_->GetHandle());
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

}
