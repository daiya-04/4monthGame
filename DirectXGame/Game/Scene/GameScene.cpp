#include "GameScene.h"
#include "DirectXCommon.h"
#include "WinApp.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>
#include "Game/Block/BlockTextureManager.h"

GameScene::~GameScene() {}

void GameScene::Init(){

	BlockTextureManager::GetInstance()->LoadAllBlockTexture();

	player_ = std::make_shared<Player>();
	player_->Initialize();

	stage_ = std::make_unique<Stage>();
	stage_->SetPlayer(player_.get());
	stage_->Initialize();
	stage_->Load(stageNumber_);
	player_->SetBlocks(stage_->GetBlocks());

	camera_ = std::make_shared<Camera>();
	camera_->Init();
	camera_->translation_ = { 0.0f,0.0f,0.0f };
	stage_->SetCamera(camera_.get());

	scroll_ = std::make_unique<Scroll>();
	scroll_->SetCamera(camera_.get());
	scroll_->Initialize();
	scroll_->SetTarget(player_->GetPositionPtr());
	

	bgTexture_ = TextureManager::Load("backGround/backGround.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));


	commandList_ = DirectXCommon::GetInstance()->GetCommandList();

	environmentEffectsManager_ = EnvironmentEffectsManager::GetInstance();
	cameraFrozen_ = CameraFrozenManager::GetInstance();
	heatHazeManager_ = HeatHazeManager::GetInstance();

	sample0.reset(new Sprite(TextureManager::GetInstance()->Load("gokkan.png"), { 640.0f,360.0f }, 7.5f));
	sample0->Initialize();
	sample1.reset(new Sprite(TextureManager::GetInstance()->Load("hhTest.png"), { 640.0f,360.0f }, 1.0f));
	sample1->Initialize();

	waterDropManager_ = WaterDropManager::GetInstance();

	snowManager_ = SnowManager::GetInstance();


	testObject_.reset(Object2d::Create(TextureManager::GetInstance()->Load("playerBlue.png"), { 1.0f,0.5f }));
	testObject_->SetSize({ 128.0f,128.0f });
}

void GameScene::Reset() {

	player_->Initialize();
	stage_->Load(stageNumber_);
	camera_->Init();
	scroll_->Initialize();

}

void GameScene::Update() {
	DebugGUI();

	//シーンチェンジしたら操作不可に
	if (!isSceneChange_) {

		if (Input::GetInstance()->TriggerKey(DIK_Q)) {
			SceneManager::GetInstance()->ChangeScene("StageSelect");
		}

		if (player_->GetIsDead() || stage_->GetIsClear()) {

			if (Input::GetInstance()->TriggerButton(Input::Button::B)) {
				Reset();
			}

		}

		preCameraPosition_ = camera_->translation_;
		if (player_->GetIsBirdsEye()) {
			scroll_->SetTarget(player_->GetBirdsEyePositionPtr());
			camera_->ChangeDrawingRange({ 1920,1080.0f });
		}
		else {
			scroll_->SetTarget(player_->GetPositionPtr());
			camera_->ChangeDrawingRange({ 1280.0f,720.0f });
		}

		stage_->Update();

		player_->Update();

		scroll_->Update();

		camera_->UpdateMatrix();

		snowManager_->SetCameraSlide({ camera_->translation_.x - preCameraPosition_.x, camera_->translation_.y - preCameraPosition_.y });
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
	else {

	}
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

	//backGround_->Draw();

}

void GameScene::DrawObject(){

	//player_->Draw(*camera_.get());

	//stage_->Draw();
	environmentEffectsManager_->Draw(commandList_);
}

void GameScene::DrawParticleModel(){



}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){
	player_->DrawUI();

	stage_->DrawUI();

}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &camera_->translation_.x, 1.0f);
	ImGui::DragFloat3("rotation", &camera_->rotation_.x, 0.01f);
	ImGui::End();

	player_->Debug();

	ImGui::Begin("IsPlayGamme");
	ImGui::Checkbox("IsPlay",&isPlayGame_);
	
	ImGui::End();

	if (Input::GetInstance()->TriggerKey(DIK_Z)) {
		isPlayGame_ = !isPlayGame_;
	};

#endif // _DEBUG
}

void GameScene::DrawCold(PostEffect* targetScene) {
	//エフェクトの描画
	if (isPlayGame_) {
		snowManager_->PreDrawUpdateEffect();
		Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		//testObject_->Draw(*camera_.get());
		player_->Draw(*camera_.get());
		snowManager_->PostDrawUpdateEffect(*(camera_.get()));
	}

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	Sprite::preDraw(commandList_);
	backGround_->Draw();
	Sprite::postDraw();
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	player_->Draw(*camera_.get());
	stage_->Draw();

	snowManager_->Draw();
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

	
}

void GameScene::DrawHeat(PostEffect* targetScene) {
	heatHazeManager_->PreDraw(commandList_);
	Sprite::preDraw(commandList_);
	backGround_->Draw();
	Sprite::postDraw();
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	player_->Draw(*camera_.get());
	stage_->Draw();
	heatHazeManager_->PostDraw(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	
	waterDropManager_->DrawScene(commandList_, heatHazeManager_->GetHandle());
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

}
