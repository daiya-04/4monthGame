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

	sample0.reset(new Sprite(TextureManager::GetInstance()->Load("gokkan.png"), {640.0f,360.0f},7.5f));
	sample0->Initialize();
	sample1.reset(new Sprite(TextureManager::GetInstance()->Load("hhTest.png"), { 640.0f,360.0f }, 1.0f));
	sample1->Initialize();
	
	waterDropManager_ = WaterDropManager::GetInstance();

	snowManager_ = SnowManager::GetInstance();

	camera_ = std::make_shared<Camera>();
	camera_->Init();
	camera_->translation_ = { 0.0f,0.0f,0.0f };

	testObject_.reset(Object2d::Create(TextureManager::GetInstance()->Load("playerBlue.png"), { 1.0f,0.5f }));
	testObject_->SetSize({128.0f,128.0f});
}

void GameScene::Update(){
	if (isPlayGame_) {
		preCameraPosition_ = camera_->translation_;
	}
	DebugGUI();

	testObject_->position_=testObjectPosition_;
	camera_->UpdateMatrix();
	if (isPlayGame_) {
		snowManager_->SetCameraSlide({ camera_->translation_.x - preCameraPosition_.x, camera_->translation_.y - preCameraPosition_.y });
		environmentEffectsManager_->Update();
	}
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
	if (isPlayGame_) {
		cameraFrozen_->DrawInternal(commandList_);
		waterDropManager_->DrawEffectUpdate(cameraFrozen_->GetEffectTexture());
	}
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

	ImGui::Begin("testObkect");
	ImGui::DragFloat2("position", &testObjectPosition_.x, 1.0f);
	ImGui::DragFloat2("cameraPos", &camera_->translation_.x, 1.0f);
	ImGui::End();

	ImGui::Begin("IsPlayGamme");
	ImGui::Checkbox("IsPlay",&isPlayGame_);
	
	ImGui::End();

	if (Input::GetInstance()->TriggerKey(DIK_Q)) {
		isPlayGame_ = !isPlayGame_;
	};

#endif // _DEBUG
}

void GameScene::DrawCold(PostEffect* targetScene) {
	//エフェクトの描画
	if (isPlayGame_) {
		snowManager_->PreDrawUpdateEffect();
		Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		testObject_->Draw(*camera_.get());
		snowManager_->PostDrawUpdateEffect(*(camera_.get()));
	}
	//cameraFrozen_->DrawInternal(commandList_);
	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	Sprite::preDraw(commandList_);
	sample0->Draw();
	Sprite::postDraw();
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	testObject_->Draw(*camera_.get());
	snowManager_->Draw();
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

	
}

void GameScene::DrawHeat(PostEffect* targetScene) {
	//cameraFrozen_->DrawInternal(commandList_);
	heatHazeManager_->PreDraw(commandList_);
	Sprite::preDraw(commandList_);
	sample1->Draw();
	Sprite::postDraw();
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	testObject_->Draw(*camera_.get());
	heatHazeManager_->PostDraw(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	//heatHazeManager_->Draw(commandList_);
	waterDropManager_->DrawScene(commandList_, heatHazeManager_->GetHandle());
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

}
