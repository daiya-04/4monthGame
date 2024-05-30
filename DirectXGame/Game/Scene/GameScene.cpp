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
#include "Text/TextManager.h"
#include "GameText/GameTextManager.h"
#include "AudioManager.h"
#include "System/TutorialFlagManager.h"

GameScene::~GameScene() {
	magmaBGM_->StopSound();
}

void GameScene::Init(){

	BlockTextureManager::GetInstance()->LoadAllBlockTexture();

	scoreManager_ = ScoreManager::GetInstance();
	score_.Init(scorePosition_, { 64.0f,64.0f });
	BaseBlock::SetScore(&score_);

	currentStageNumber_ = stageNumber_;

	player_ = std::make_shared<Player>();
	player_->Initialize();

	stage_ = std::make_unique<Stage>();
	stage_->SetPlayer(player_.get());
	stage_->Initialize(stageNumber_);
	player_->SetBlocks(stage_->GetBlocks());

	camera_ = std::make_shared<Camera>();
	camera_->Init();
	camera_->translation_ = { 0.0f,0.0f,0.0f };
	camera_->ChangeDrawingRange({ 1600.0f,900.0f });
	stage_->SetCamera(camera_.get());

	scrollHomePoint_ = Stage::kBasePosition + Vector2{ 0.0f,-600.0f };
	isScrollEnd_ = false;
	scroll_ = std::make_unique<Scroll>();
	scroll_->SetCamera(camera_.get());
	scroll_->Initialize();
	scroll_->SetTarget(&scrollHomePoint_);
	scroll_->SetCameraOnTarget();

	bgTexture_ = TextureManager::Load("backGround/backGround.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));


	commandList_ = DirectXCommon::GetInstance()->GetCommandList();

	environmentEffectsManager_ = EnvironmentEffectsManager::GetInstance();
	//stageに現在のシーンモードを適応する
	AppryMode();

	cameraFrozen_ = CameraFrozenManager::GetInstance();
	heatHazeManager_ = HeatHazeManager::GetInstance();

	sample0.reset(new Sprite(TextureManager::GetInstance()->Load("gokkan.png"), { 640.0f,360.0f }, 7.5f));
	sample0->Initialize();
	sample1.reset(new Sprite(TextureManager::GetInstance()->Load("hhTest.png"), { 640.0f,360.0f }, 1.0f));
	sample1->Initialize();

	waterDropManager_ = WaterDropManager::GetInstance();

	snowManager_ = SnowManager::GetInstance();

	backGameTex_ = TextureManager::Load("UI/tmpMenu.png");
	menuButtonTex_ = TextureManager::Load("UI/menu.png");

	backSprite_.reset(Sprite::Create(backGameTex_, { 640.0f, 360.0f }));
	/*restartSprite_.reset(Sprite::Create(restartTex_, { 640.0f, 350.0f }));
	toStageSelectSprite_.reset(Sprite::Create(toStageSelectTex_, { 640.0f, 550.0f }));*/
	menuButtonSprite_.reset(Sprite::Create(menuButtonTex_, { 1150.0f, 690.0f }));

	testObject_.reset(Object2d::Create(TextureManager::GetInstance()->Load("player/playerBlue.png"), { 1.0f,0.5f }));
	testObject_->SetSize({ 128.0f,128.0f });
	isFirstAllDraw_ = true;
	TextManager::GetInstance();

	magmaBGM_ = AudioManager::GetInstance()->Load("BGM/magmaBGM.mp3");
	
	magmaBGM_->Play();

	TextManager::GetInstance();
	testText_.reset(new Text);
	testText_->Initialize();
	testText_->SetWString(L"AAAAA");
	GameTextManager::GetInstance()->InitializeStage(999);

	TutorialFlagManager::GetInstance()->Initialize();
	TutorialFlagManager::GetInstance()->SetPlayer(player_.get());
	TutorialFlagManager::GetInstance()->SetMagma(stage_->GetMagma());

}

void GameScene::Reset() {

	player_->Initialize();
	stage_->Load(stageNumber_);
	camera_->Init();
	scroll_->Initialize();
	scroll_->SetCameraOnTarget();
	TutorialFlagManager::GetInstance()->Initialize();

}

void GameScene::Update() {
	DebugGUI();
	TextManager::GetInstance()->ClearText();
	GameTextManager::GetInstance()->Update();
#ifdef _DEBUG

	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_1)) {
		SceneManager::GetInstance()->ChangeScene("Title");
	}
	if (Input::GetInstance()->PushKey(DIK_LCONTROL) && Input::GetInstance()->TriggerKey(DIK_2)) {
		scoreManager_->SetScore(currentStageNumber_, score_);
		scoreManager_->SaveScore();
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}

#endif // _DEBUG

	isPreOpenMenu_ = isOpenMenu_;

	//スタート時のスクロール更新
	if (scrollHomePoint_.y < -200.0f) {
		
		scrollHomePoint_.y += (- 200.0f - scrollHomePoint_.y) * 0.1f;

		if (scrollHomePoint_.y > -201.0f) {
			scrollHomePoint_.y = -200.0f;
			isScrollEnd_ = true;
			GameTextManager::GetInstance()->InitializeStage(stageNumber_);
		}

	}

	if (stage_->GetIsClear()) {

		if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
			ClearProcess();
			SceneManager::GetInstance()->ChangeScene("StageSelect");
		}

	}
	else if (isOpenMenu_ && isPreOpenMenu_) {

		if (Input::GetInstance()->TriggerButton(Input::Button::B) ||
			Input::GetInstance()->TriggerButton(Input::Button::START)) {
			isOpenMenu_ = false;
		}

		/*if (Input::GetInstance()->TriggerButton(Input::Button::A)) {

			switch (menu_)
			{
			default:
			case GameScene::kBack:
				isOpenMenu_ = false;
				menu_ = kBack;
				break;
			case GameScene::kRestart:
				SceneManager::GetInstance()->ChangeScene("Game");
				break;
			case GameScene::kStageSelect:
				SceneManager::GetInstance()->ChangeScene("StageSelect");
				break;
			}

		}*/

	}
	else {

		if (GameTextManager::GetInstance()->GetIsEnd() && isScrollEnd_) {

			if (stageNumber_ == 1) {
				TutorialFlagManager::GetInstance()->Update();
			}

			if (Input::GetInstance()->TriggerButton(Input::Button::START)) {
				isOpenMenu_ = true;
			}

			stage_->Update();

			preCameraPosition_ = camera_->translation_;
			if (player_->GetIsBirdsEye()) {
				scroll_->SetScrollType(0, Scroll::kNomral);
				scroll_->SetScrollType(1, Scroll::kNomral);
				scroll_->SetTarget(player_->GetBirdsEyePositionPtr());
				camera_->ChangeDrawingRange({ 2240,1260.0f });
				snowManager_->ClearEffect();
			}
			else if (player_->GetIsHome()) {
				scroll_->SetScrollType(0, Scroll::kDelay);
				scroll_->SetScrollType(1, Scroll::kDelay);
				scroll_->SetTarget(&scrollHomePoint_);
				camera_->ChangeDrawingRange({ 1600.0f,900.0f });
			}
			else {
				scroll_->SetScrollType(0, Scroll::kDelay);
				scroll_->SetScrollType(1, Scroll::kDelay);
				scroll_->SetTarget(player_->GetPositionPtr());
				camera_->ChangeDrawingRange({ 1600.0f,900.0f });
			}

			//アップグレード中は動けない
			if (!stage_->GetIsActiveUpgrade()) {
				player_->Update();
			}

			player_->UpdateUI();

		}

	}

	scroll_->Update();

	camera_->UpdateMatrix();
	//湧き上がる温泉の熱風のエフェクト
	float line = stage_->GetMagmaLine();
	cameraFrozen_->CulcFrozenArea(player_->GetPosition(Player::kRightBottom).y, line);
	heatHazeManager_->SetMagmaLineScreen(Transform(Vector3{ 0.0f,line,0.0f }, (camera_->GetMatView() * camera_->GetMatProjection())));
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

void GameScene::ClearProcess() {

	//ハイスコア更新で記録を塗り替える
	scoreManager_->SetScore(currentStageNumber_, score_);

}

void GameScene::DrawNotSetPipeline() {

	//ゲーム中のオーバーヘッドを回避するため最初に一度すべて描画する
	if (isFirstAllDraw_) {
		cameraFrozen_->DrawInternal(commandList_);
		waterDropManager_->DrawEffectUpdate(cameraFrozen_->GetEffectTexture());
		DrawHeat(environmentEffectsManager_->GetPrevScene());
		DrawCold(environmentEffectsManager_->GetNextScene());
		isFirstAllDraw_ = false;
	}
	//stage_->Update();

	if (isPlayGame_) {
		cameraFrozen_->DrawInternal(commandList_);
		waterDropManager_->DrawEffectUpdate(cameraFrozen_->GetEffectTexture());
	}
	stage_->Draw();
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

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){
	player_->DrawUI();

	stage_->DrawUI();

	score_.Draw();

	if (isOpenMenu_) {

		backSprite_->Draw();

	}
	else {

		if (GameTextManager::GetInstance()->GetIsEnd() && isScrollEnd_) {
			menuButtonSprite_->Draw();
		}

	}

	//TextManager::GetInstance()->TestDraw();
	//TextManager::GetInstance()->TestDraw();
	GameTextManager::GetInstance()->Draw();
	testText_->SetText();
	TextManager::GetInstance()->Draw();
}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	/*ImGui::Begin("camera");
	ImGui::DragFloat3("translation", &camera_->translation_.x, 1.0f);
	ImGui::DragFloat3("rotation", &camera_->rotation_.x, 0.01f);
	ImGui::End();*/

	player_->Debug();

	if (Input::GetInstance()->TriggerKey(DIK_9)) {
		ChangeMode();
	}

	//GameTextManager::GetInstance()->TestUpdate();

	static int cCount;
	static Vector2 position;
	ImGui::Begin("testText");
	ImGui::DragInt("CharCount", &cCount, 1,0,100);
	ImGui::DragFloat2("originPosition", &position.x,1.0f);
	static int32_t tutorial;
	if (ImGui::Button("DrawTutorialText!") && GameTextManager::GetInstance()->GetIsEnd()) {
		tutorial++;
		GameTextManager::GetInstance()->Tutorial(tutorial);
	}
	ImGui::End();
	testText_->SetArrangeType(Text::kCenter);
	testText_->SetCharCount(uint32_t(cCount));
	testText_->SetPosition(position);
	//TextManager::GetInstance()->SetCharCount(uint32_t(cCount));
	TextManager::GetInstance()->OffsetEditorDraw();

#endif // _DEBUG
}

void GameScene::DrawCold(PostEffect* targetScene) {
	//エフェクトの描画
	/*if (isPlayGame_) {
		snowManager_->PreDrawUpdateEffect();
		Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		//testObject_->Draw(*camera_.get());
		player_->Draw(*camera_.get());
		snowManager_->PostDrawUpdateEffect(*(camera_.get()));
	}*/

	heatHazeManager_->PreDrawMagma(commandList_);
	Sprite::preDraw(commandList_);
	backGround_->Draw();
	Sprite::postDraw();
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());

	stage_->DrawColdBefore();

	if (!player_->GetIsDead()) {
		player_->Draw(*camera_.get());
	}
	stage_->DrawColdAfter();
	if (player_->GetIsDead()) {
		player_->Draw(*camera_.get());
	}

	//snowManager_->Draw();
	heatHazeManager_->PostDrawMagma(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	heatHazeManager_->DrawMagma(commandList_);
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);	
}

void GameScene::DrawHeat(PostEffect* targetScene) {
	heatHazeManager_->PreDraw(commandList_);
	Sprite::preDraw(commandList_);
	backGround_->Draw();
	Sprite::postDraw();
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	
	stage_->DrawHeatBefore();

	if (!player_->GetIsDead()) {
		player_->Draw(*camera_.get());
	}
	stage_->DrawHeatAfter();
	if (player_->GetIsDead()) {
		player_->Draw(*camera_.get());
	}

	heatHazeManager_->PostDraw(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	
	waterDropManager_->DrawScene(commandList_, heatHazeManager_->GetHandle());
	cameraFrozen_->Draw(commandList_);
	targetScene->PostDrawScene(commandList_);

}


void GameScene::ChangeMode() {
	environmentEffectsManager_->ChangeSceneMode();
	AppryMode();
}

void GameScene::AppryMode() {
	bool sceneMode = environmentEffectsManager_->GetIsNowScene();
	if ((sceneMode) ^ environmentEffectsManager_->GetIsPlaySceneChangeAnimation() ) {
		stage_->ChangeSnow2Magma();
	}
	else {
		stage_->ChangeMagma2Snow();
	}
}