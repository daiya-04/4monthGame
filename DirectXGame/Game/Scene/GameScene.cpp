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
	coldBGM_->StopSound();
}

void GameScene::Init(){

	//ゲームシーンでインスタンスを持っておく
	EnemyManager::GetInstance();

	BlockTextureManager::GetInstance()->LoadAllBlockTexture();

	scoreManager_ = ScoreManager::GetInstance();
	score_.Init(scorePosition_, { 48.0f,48.0f });
	score_.SetSpace(32.0f);
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
	environmentEffectsManager_->SetCamera(camera_.get());

	cameraFrozen_ = CameraFrozenManager::GetInstance();
	cameraFrozen_->ClearEffect();
	cameraFrozen_->Reset();

	//1~3,7~9は灼熱開始
	if (stageNumber_ <= 3 || stageNumber_ > 6) {

		environmentEffectsManager_->InitializeStage(true);
		cameraFrozen_->Reset();
		//テクスチャを氷に変更
		BlockTextureManager::GetInstance()->ChangeTexture(Block::kDownMagma, 5);

	}
	//4~6は極寒開始
	else if (stageNumber_ <= 6) {

		environmentEffectsManager_->InitializeStage(false);
		cameraFrozen_->Start();
		//テクスチャをマグマに変更
		BlockTextureManager::GetInstance()->ChangeTexture(Block::kDownMagma, 4);

	}
	//stageに現在のシーンモードを適応する
	AppryMode();

	heatHazeManager_ = HeatHazeManager::GetInstance();

	waterDropManager_ = WaterDropManager::GetInstance();

	snowManager_ = SnowManager::GetInstance();

	backGameTex_ = TextureManager::Load("UI/tmpMenu.png");
	menuButtonTex_ = TextureManager::Load("UI/menu.png");
	toGameTex_ = TextureManager::Load("UI/toGame.png");
	optionTex_ = TextureManager::Load("UI/option.png");
	toStageSelectTex_ = TextureManager::Load("UI/toStageSelect.png");
	scoreFrontTex_ = TextureManager::Load("UI/scoreFrame.png");
	scoreMiddleTex_ = TextureManager::Load("UI/scoreGage.png");
	scoreBackTex_ = TextureManager::Load("UI/scoreBackGround.png");
	clearBGTex_ = TextureManager::Load("UI/clearBG.png");
	clearTex_ = TextureManager::Load("UI/gameClear.png");
	numTex_ = TextureManager::Load("UI/number.png");

	menuBackSprite_.reset(Sprite::Create(backGameTex_, { 640.0f, 360.0f }));
	menuButtonSprite_.reset(Sprite::Create(menuButtonTex_, { 1150.0f, 690.0f }));
	menuSprites_[kBack].reset(Sprite::Create(toGameTex_, { 840.0f, 160.0f }));
	menuSprites_[kBack]->SetSize({ 512.0f,64.0f });
	menuSprites_[kOption].reset(Sprite::Create(optionTex_, { 840.0f, 360.0f }));
	menuSprites_[kOption]->SetSize({ 512.0f,64.0f });
	menuSprites_[kStageSelect].reset(Sprite::Create(toStageSelectTex_, { 840.0f, 560.0f }));
	menuSprites_[kStageSelect]->SetSize({ 512.0f,64.0f });
	scoreFront_.reset(Sprite::Create(scoreFrontTex_, { 250.0f, 64.0f }));
	/*scoreGage_.reset(Sprite::Create(scoreMiddleTex_, { 106.0f, 64.0f }));*/
	scoreGage_.reset(Sprite::Create(scoreMiddleTex_, { 106.0f, 64.0f }));
	scoreGage_->SetAnchorpoint({ 0.0f,0.5f });
	scoreGage_->SetSize({ 380.0f,96.0f});
	scoreGage_->SetTextureArea({ 0.0f,0.0f }, { 380.0f,96.0f });
	scoreBack_.reset(Sprite::Create(scoreBackTex_, { 250.0f, 64.0f }));
	clearBGSprite_.reset(Sprite::Create(clearBGTex_, { 640.0f, 360.0f }));
	clearBGSprite_->SetColor({ 1.0f,1.0f,1.0f,clearBGAlpha_ });
	clearSprite_.reset(Sprite::Create(clearTex_, { 640.0f,360.0f }));
	
	for (int32_t i = 0; i < 2; i++) {
		changeCount_[i].reset(Object2d::Create(numTex_, player_->GetPosition() - Vector2{16.0f + 32.0f * i,96.0f}));
		changeCount_[i]->SetSize({ 64.0f,64.0f });
		changeCount_[i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });
	}

	isFirstAllDraw_ = true;
	TextManager::GetInstance();

	magmaBGM_ = AudioManager::GetInstance()->Load("BGM/magmaBGM.mp3");
	coldBGM_ = AudioManager::GetInstance()->Load("BGM/coldBGM.mp3");

	magmaBGM_->Play();
	coldBGM_->Play();

	if (environmentEffectsManager_->GetIsNowScene()) {
		magmaBGM_->SetUniqueVolume(1.0f);
		coldBGM_->SetUniqueVolume(0.0f);
	}
	else {
		magmaBGM_->SetUniqueVolume(0.0f);
		coldBGM_->SetUniqueVolume(1.0f);
	}

	magmaBGM_->Update();
	coldBGM_->Update();

	selectSE_ = AudioManager::GetInstance()->Load("SE/select_ok.mp3");
	moveSE_ = AudioManager::GetInstance()->Load("SE/select_move.mp3");
	cancelSE_ = AudioManager::GetInstance()->Load("SE/select_cancel.mp3");

	TextManager::GetInstance();
	testText_.reset(new Text);
	testText_->Initialize();
	testText_->SetWString(L"AAAAA");
	GameTextManager::GetInstance()->InitializeStage(999);

	TutorialFlagManager::GetInstance()->Initialize();
	TutorialFlagManager::GetInstance()->SetPlayer(player_.get());
	TutorialFlagManager::GetInstance()->SetMagma(stage_->GetMagma());
	TutorialFlagManager::GetInstance()->SetScroll(scroll_.get());

	option_ = std::make_unique<Option>();

	transitionEffect_ = std::make_unique<TransitionEffect>();
	transitionEffect_->Init();

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
			stage_->SetIsStart(true);
			GameTextManager::GetInstance()->InitializeStage(stageNumber_);
		}

	}

	if (stage_->GetIsClear()) {

		if (clearBGAlpha_ < 1.0f) {

			clearBGAlpha_ += 1.0f / 30.0f;

			if (clearBGAlpha_ > 1.0f) {
				clearBGAlpha_ = 1.0f;
			}

		}

		float rotate = clearBGSprite_->GetRotate();
		rotate += 0.02f;

		if (rotate > 6.28f) {
			rotate = 0.0f;
		}

		clearBGSprite_->SetRotate(rotate);
		clearBGSprite_->SetColor({ 1.0f,1.0f,1.0f,clearBGAlpha_ });

		if (clearBGAlpha_ >= 1.0f && Input::GetInstance()->TriggerButton(Input::Button::A)) {
			selectSE_->Play();
			ClearProcess();
			SceneManager::GetInstance()->ChangeScene("StageSelect");
		}

	}
	else if (isOpenMenu_ && isPreOpenMenu_) {

		//オプションを開いていなかったら
		if (!option_->IsWindow()) {

			if (Input::GetInstance()->TriggerButton(Input::Button::B) ||
				Input::GetInstance()->TriggerButton(Input::Button::START)) {
				isOpenMenu_ = false;
				menu_ = kBack;
				cancelSE_->Play();
			}

			if (Input::GetInstance()->TriggerButton(Input::Button::DPAD_UP) ||
				Input::GetInstance()->TriggerLStick(Input::Stick::Up)) {

				if (menu_ == kOption) {
					menu_ = kBack;
					menuSprites_[kBack]->SetPosition({ 940.0f,160.0f });
					BoundingInit(menuSprites_[kBack]->GetPosition());
				}
				else if (menu_ == kStageSelect) {
					menu_ = kOption;
					menuSprites_[kOption]->SetPosition({ 940.0f,360.0f });
					BoundingInit(menuSprites_[kOption]->GetPosition());
				}

				moveSE_->Play();

			}
			else if (Input::GetInstance()->TriggerButton(Input::Button::DPAD_DOWN) ||
				Input::GetInstance()->TriggerLStick(Input::Stick::Down)) {

				if (menu_ == kBack) {
					menu_ = kOption;
					menuSprites_[kOption]->SetPosition({ 940.0f,360.0f });
					BoundingInit(menuSprites_[kOption]->GetPosition());
				}
				else if (menu_ == kOption) {
					menu_ = kStageSelect;
					menuSprites_[kStageSelect]->SetPosition({ 940.0f,560.0f });
					BoundingInit(menuSprites_[kStageSelect]->GetPosition());
				}

				moveSE_->Play();

			}

			if (Input::GetInstance()->TriggerButton(Input::Button::A)) {

				switch (menu_)
				{
				default:
				case GameScene::kBack:
					isOpenMenu_ = false;
					menu_ = kBack;
					cancelSE_->Play();
					break;
				case GameScene::kOption:
					option_->Init();
					selectSE_->Play();
					break;
				case GameScene::kStageSelect:
					selectSE_->Play();
					SceneManager::GetInstance()->ChangeScene("StageSelect");
					break;
				}

			}

			switch (menu_)
			{
			default:
			case GameScene::kBack:
				//menuSprites_[kBack]->SetPosition({ 940.0f,160.0f });
				BoundingUpdate(menuSprites_[kBack].get());
				menuSprites_[kOption]->SetPosition({ 1040.0f,360.0f });
				menuSprites_[kStageSelect]->SetPosition({ 1040.0f,560.0f });
				break;
			case GameScene::kOption:
				menuSprites_[kBack]->SetPosition({ 1040.0f,160.0f });
				//menuSprites_[kOption]->SetPosition({ 940.0f,360.0f });
				BoundingUpdate(menuSprites_[kOption].get());
				menuSprites_[kStageSelect]->SetPosition({ 1040.0f,560.0f });
				break;
			case GameScene::kStageSelect:
				menuSprites_[kBack]->SetPosition({ 1040.0f,160.0f });
				menuSprites_[kOption]->SetPosition({ 1040.0f,360.0f });
				//menuSprites_[kStageSelect]->SetPosition({ 940.0f,560.0f });
				BoundingUpdate(menuSprites_[kStageSelect].get());
				break;
			}

		}

		option_->Update();

	}
	else {

		if (player_->GetIsBirdsEye()) {
			scroll_->SetScrollType(0, Scroll::kNomral);
			scroll_->SetScrollType(1, Scroll::kNomral);
			scroll_->SetTarget(player_->GetBirdsEyePositionPtr());
			camera_->ChangeDrawingRange({ 2240,1260.0f });
			heatHazeManager_->SetIsUseMagmaFluction(false);
			snowManager_->ClearEffect();
		}
		else if (player_->GetIsHome()) {
			scroll_->SetScrollType(0, Scroll::kDelay);
			scroll_->SetScrollType(1, Scroll::kDelay);
			scroll_->SetTarget(&scrollHomePoint_);
			camera_->ChangeDrawingRange({ 1600.0f,900.0f });
			heatHazeManager_->SetIsUseMagmaFluction(true);
		}
		else {
			scroll_->SetScrollType(0, Scroll::kDelay);
			scroll_->SetScrollType(1, Scroll::kDelay);
			scroll_->SetTarget(player_->GetPositionPtr());
			camera_->ChangeDrawingRange({ 1600.0f,900.0f });
			heatHazeManager_->SetIsUseMagmaFluction(true);
		}

		if (transitionEffect_->isActiveStart()) {
			transitionEffect_->OutEffectStart();
		}

		transitionEffect_->Update();

		if (transitionEffect_->InEffectEnter()) {

			if (player_->GetIsBirdsEye()) {
				player_->SetIsBirdsEye(false);
			}
			else {
				player_->SetIsBirdsEye(true);
			}

		}

		transitionEffect_->FlagUpdate();

		if (GameTextManager::GetInstance()->GetIsEnd() && isScrollEnd_ &&
			!transitionEffect_->IsActive()) {

			if (stageNumber_ == 1) {
				TutorialFlagManager::GetInstance()->Update();
			}

			if (Input::GetInstance()->TriggerButton(Input::Button::START)) {
				isOpenMenu_ = true;
				selectSE_->Play();
				menuSprites_[kBack]->SetPosition({ 940.0f,160.0f });
				BoundingInit(menuSprites_[kBack]->GetPosition());
			}

			stage_->Update();
			//heatHazeManager_->SetIsUseMagmaFluction(true);
			preCameraPosition_ = camera_->translation_;

			//アップグレード中は動けない
			if (!stage_->GetIsActiveUpgrade()) {
				
				player_->Update();

				//十回掘るごとにリセット
				if (player_->GetDigCount() >= 10) {

					//特定のステージでは極寒灼熱切り替えする
					if (stageNumber_ > 6) {
						ChangeMode();
					}
					//カウントリセット
					player_->ResetDigCount();

				}

				if (player_->GetIsStartFade()) {
					transitionEffect_->IsActiveOn();
					player_->SetIsStartFade(false);
				}

			}

			player_->UpdateUI();

		}
		else if (!GameTextManager::GetInstance()->GetIsEnd()) {

			//テキスト中に更新するもの
			if (stageNumber_ == 1) {
				TutorialFlagManager::GetInstance()->UpdateInText();
			}

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
	
	//スコアゲージ調整
	UpdateScoreGage();
	
	if (stageNumber_ > 6) {

		for (int32_t i = 0; i < 2; i++) {

			int32_t num = 0;

			int32_t divide = int32_t(std::pow(10, 2 - 1 - i));

			num = (10 - player_->GetDigCount()) / divide;

			changeCount_[i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

			changeCount_[i]->position_ = player_->GetPosition() + Vector2{ -16.0f + 32.0f * i,-96.0f };

		}

	}

	score_.AddScoreEffectUpdate();

}

void GameScene::UpdateScoreGage() {

	if (ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) > 0) {

		//倍率設定
		float gageMag = float(score_.GetScore()) /
			(float(ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1)) * 1.15f);
		//1.0fを超えないようにする
		gageMag = std::clamp(gageMag, 0.0f, 1.0f);

		scoreGage_->SetSize({ 380.0f * gageMag,96.0f });
		scoreGage_->SetTextureArea({ 0.0f,0.0f },
			{ 380.0f * gageMag, 96.0f });

		//ゲージの色変化

		//S、黄金色
		if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1)) {
			scoreGage_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
		}
		//A、赤色
		else if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) / 4 * 3) {
			scoreGage_->SetColor({ 1.0f,0.3f,0.3f,1.0f });
		}
		//B、黄色
		else if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) / 2) {
			scoreGage_->SetColor({ 1.0f,1.0f,0.3f,1.0f });
		}
		//C、緑色
		else if (score_.GetScore() >= ScoreManager::GetInstance()->GetMaxRankScore(stageNumber_ - 1) / 4) {
			scoreGage_->SetColor({ 0.3f,1.0f,0.3f,1.0f });
		}
		//D、水色
		else {
			scoreGage_->SetColor({ 0.3f,1.0f,1.0f,1.0f });
		}

	}
	else {
		scoreGage_->SetSize({ 422.0f,96.0f });
		scoreGage_->SetTextureArea({ 0.0f,0.0f },
			{ 422.0f, 96.0f });
		scoreGage_->SetColor({ 1.0f,1.0f,0.7f,1.0f });
	}

}

void GameScene::ClearProcess() {

	//ハイスコア更新で記録を塗り替える
	scoreManager_->SetScore(currentStageNumber_, score_);
	scoreManager_->SaveScore();

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
	//ヒットエフェクト
	BlockTextureManager::GetInstance()->DrawHitEffect(*camera_.get());
	BlockTextureManager::GetInstance()->DrawChargeCompleteUI(*camera_.get());
}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){

	if (isOpenMenu_) {

		if (!option_->IsWindow()) {

			menuBackSprite_->Draw();

			for (int32_t i = 0; i < kMaxMenu; i++) {
				menuSprites_[i]->Draw();
			}

		}

		option_->Draw();

	}
	else {

		player_->DrawUI();

		stage_->DrawUI();

		scoreBack_->Draw();
		scoreGage_->Draw();
		scoreFront_->Draw();
		score_.Draw();

		BlockTextureManager::GetInstance()->DrawParticleUI();

		if (GameTextManager::GetInstance()->GetIsEnd() && isScrollEnd_) {
			menuButtonSprite_->Draw();
		}

	}

	//TextManager::GetInstance()->TestDraw();
	//TextManager::GetInstance()->TestDraw();
	GameTextManager::GetInstance()->Draw();
	testText_->SetText();
	TextManager::GetInstance()->Draw();

	if (stage_->GetIsClear()) {

		if (clearBGAlpha_ >= 1.0f) {
			clearSprite_->Draw();
		}

	}

	if (transitionEffect_->IsActive()) {
		transitionEffect_->Draw();
	}

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

	if (stage_->GetIsClear()) {
		clearBGSprite_->Draw();
	}

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

	if (stageNumber_ > 6 && player_->GetIsMine()) {

		for (int32_t i = 0; i < 2; i++) {
			changeCount_[i]->Draw(*camera_.get());
		}

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

	if (stage_->GetIsClear()) {
		clearBGSprite_->Draw();
	}

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

	if (stageNumber_ > 6 && player_->GetIsMine()) {

		for (int32_t i = 0; i < 2; i++) {
			changeCount_[i]->Draw(*camera_.get());
		}

	}


	heatHazeManager_->PostDraw(commandList_);

	//取得したシーンに対して描画
	targetScene->PreDrawScene(commandList_);
	
	waterDropManager_->DrawScene(commandList_, heatHazeManager_->GetHandle());
	if (environmentEffectsManager_->GetIsPlaySceneChangeAnimation()) {
		cameraFrozen_->Draw(commandList_);
	}
	targetScene->PostDrawScene(commandList_);

}


void GameScene::ChangeMode() {

	if (!environmentEffectsManager_->GetIsPlaySceneChangeAnimation() && !environmentEffectsManager_->GetIsPlayDownMagma()) {

		//極寒だったら氷ブロックを消す処理をする
		if (!environmentEffectsManager_->GetIsNowScene()) {
			stage_->BreakIceBlock();
			//テクスチャを氷に変更
			BlockTextureManager::GetInstance()->ChangeTexture(Block::kDownMagma, 5);
			//マグマBGMに切り替え
			magmaBGM_->SetUniqueVolume(1.0f);
			coldBGM_->SetUniqueVolume(0.0f);
		}
		//灼熱から切り替わった瞬間に氷追加
		else {
			stage_->CreateIceBlock();
			//テクスチャをマグマに変更
			BlockTextureManager::GetInstance()->ChangeTexture(Block::kDownMagma, 4);
			//極寒BGMに切り替え
			magmaBGM_->SetUniqueVolume(0.0f);
			coldBGM_->SetUniqueVolume(1.0f);
		}

		environmentEffectsManager_->ChangeSceneMode();

		AppryMode();
	}
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

void GameScene::BoundingInit(const Vector2& axisPos) {

	workBounding_.axisPos_ = axisPos;
	workBounding_.velocity_.x = 1.5f;
	workBounding_.accel_ = {};
	workBounding_.addPos_ = {};

}

void GameScene::BoundingUpdate(Sprite* sprite) {

	workBounding_.accel_.x += -0.01f;

	workBounding_.velocity_.x += workBounding_.accel_.x;

	workBounding_.addPos_ += workBounding_.velocity_;

	if (workBounding_.addPos_.x < 0) {
		BoundingInit(workBounding_.axisPos_);
	}

	sprite->SetPosition(workBounding_.axisPos_ + workBounding_.addPos_);

}
