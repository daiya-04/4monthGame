#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
#include <vector>
#include <array>

#include "Sprite.h"
#include "Object2d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Stage/Stage.h"
#include "Player/Player.h"
#include "Scroll/Scroll.h"

#include "Engine/Loader/LevelLoader.h"
#include "PostEffect.h"
#include "DualSceneDrawer.h"
#include "Sprite.h"
#include "WeightCircle/WeightCircle.h"
#include "EnvironmentEffects/HeatHaze/HeatHazeManager.h"
#include "EnvironmentEffects/CameraFrozen/CameraFrozenManager.h"
#include "EnvironmentEffects/EnvironmentEffectsManager.h"
#include "EnvironmentEffects/WaterDrop/WaterDropManager.h"
#include "EnvironmentEffects/Snow/SnowManager.h"
#include "TransitionEffect.h"

#include "Text/Text.h"
#include "Option.h"

class GameScene : public DefaultScene {
public:

	void Init()override;

	void Update()override;

	void DrawNotSetPipeline()override;//

	void DrawBackGround()override;

	void DrawObject()override;

	void DrawParticle()override;

	void DrawUI()override;

	void DebugGUI()override;

	~GameScene()override;
	
	/// <summary>
	/// 灼熱状態のシーン描画
	/// </summary>
	/// <param name="targetScene">書き込み先</param>
	void DrawHeat(PostEffect* targetScene);

	/// <summary>
	/// 極寒状態のシーン描画
	/// </summary>
	/// <param name="targetScene">書き込み先</param>
	void DrawCold(PostEffect* targetScene);

	//極寒,灼熱モードを切り替える
	void ChangeMode();

	//stage側にeffectManagerが持つモードを適応する
	void AppryMode();

	//スコアゲージ更新
	void UpdateScoreGage();

private:
	ID3D12GraphicsCommandList* commandList_;
	
	HeatHazeManager* heatHazeManager_;
	CameraFrozenManager* cameraFrozen_;
	EnvironmentEffectsManager* environmentEffectsManager_;

	enum MenuType {
		kBack, //ゲームに戻る
		kOption, //オプション
		kStageSelect, //ステージ選択へ

		kMaxMenu, 
	};

	WaterDropManager* waterDropManager_;
	SnowManager* snowManager_;
	float offset_;
	float roop_;
	float width_;

	void Reset();

	//クリア時の処理
	void ClearProcess();

	ScoreManager* scoreManager_ = nullptr;
	Score score_;

	std::unique_ptr<Scroll> scroll_;
	Vector2 scrollHomePoint_{};
	bool isScrollEnd_ = false;

	Vector2 scorePosition_ = { 240.0f,96.0f };

	//UI関連
	std::unique_ptr<Sprite> backGround_;
	std::unique_ptr<Sprite> pose_;
	std::unique_ptr<Sprite> scoreFront_;
	std::unique_ptr<Sprite> scoreGage_;
	std::unique_ptr<Sprite> scoreBack_;
	std::unique_ptr<Sprite> clearBGSprite_;
	std::unique_ptr<Sprite> clearSprite_;

	std::unique_ptr<TransitionEffect> transitionEffect_;

	uint32_t bgTexture_;
	uint32_t poseTex_;

	std::shared_ptr<Camera> camera_;
	std::shared_ptr<Object2d> testObject_;//雪テスト用
	Vector2 testObjectPosition_ = {640.0f,350.0f};
	Vector3 preCameraPosition_ = {0,0,0};

	bool isPlayGame_ = true;//ゲームを再生するかどうか
	bool isFirstAllDraw_;

	bool isStartClearEffect_ = false;
	float clearBGAlpha_ = 0.0f;

///メニュー関係--------------------------------

	//メニューを開いているか
	bool isOpenMenu_ = false;
	//入力受付調整用
	bool isPreOpenMenu_ = false;

	MenuType menu_ = kBack;

	std::unique_ptr<Option> option_;

	std::unique_ptr<Sprite> menuBackSprite_;

	std::unique_ptr<Sprite> menuButtonSprite_;

	std::array<std::unique_ptr<Sprite>, kMaxMenu> menuSprites_;

	uint32_t backGameTex_;
	uint32_t restartTex_;
	uint32_t toGameTex_;
	uint32_t optionTex_;
	uint32_t toStageSelectTex_;
	uint32_t menuButtonTex_;
	uint32_t scoreFrontTex_;
	uint32_t scoreMiddleTex_;
	uint32_t scoreBackTex_;
	uint32_t clearBGTex_;
	uint32_t clearTex_;

///-------------------------------------------

	Audio* magmaBGM_;
	Audio* coldBGM_;

	Audio* selectSE_;
	Audio* moveSE_;
	Audio* cancelSE_;

	uint32_t currentStageNumber_ = 0;

	std::unique_ptr<Text> testText_;
};

