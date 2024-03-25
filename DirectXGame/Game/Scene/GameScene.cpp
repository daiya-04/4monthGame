#include "GameScene.h"
#include "DirectXCommon.h"
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
	
}

void GameScene::Update(){
	DebugGUI();
	environmentEffectsManager_->Update();
	heatHazeManager_->Update();
	if (environmentEffectsManager_->GetIsChangeComplete()) {
		if (!environmentEffectsManager_->GetIsNowScene()) {
			cameraFrozen_->Start();
		}
		else {
			cameraFrozen_->Reset();
		}
	}
	cameraFrozen_->Update();
}

void GameScene::DrawNotSetPipeline() {
	//極寒状態だったら
	if (!environmentEffectsManager_->GetIsNowScene()) {
		DrawCold(environmentEffectsManager_->GetPrevScene());
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
	
	//DrawHeat(nextScene.get());

	if (environmentEffectsManager_->GetIsPlaySceneChangeAnimation()) {
		environmentEffectsManager_->WeightCircleDraw();
	}
	//weightCircle_->Draw();

	//DirectXCommon::GetInstance()->preDraw();
	

}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawModel(){

	

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
	cameraFrozen_->DrawInternal(commandList_);
	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	Sprite::preDraw(commandList_);
	sample0->Draw();
	Sprite::postDraw();
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);


}

void GameScene::DrawHeat(PostEffect* targetScene) {
	heatHazeManager_->PreDraw(commandList_);
	Sprite::preDraw(commandList_);
	sample1->Draw();
	Sprite::postDraw();
	heatHazeManager_->PostDraw(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	heatHazeManager_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

}
