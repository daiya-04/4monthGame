#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
#include <vector>
#include <array>
#include <unordered_map>
#include <optional>

#include "Sprite.h"
#include "Object2d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"


class StageSelectScene : public DefaultScene {
public:

	void Init()override;

	void Update()override;

	void DrawBackGround()override;

	void DrawObject()override;

	void DrawParticle()override;

	void DrawNotSetPipeline()override {};

	void DrawUI()override;

	void DebugGUI()override;

	~StageSelectScene()override;

	void LoadData();

	void ApplyGlobalVariables();

private:

	Camera camera_;

	const std::string dataName = "StageSelectUI";

	char filePathBuff[256]{};
	char groupNameBuff[256]{};
	Vector2 addPos{};

	uint32_t bgTexture_;
	std::unique_ptr<Sprite> backGround_;

	std::unordered_map<std::string, std::unique_ptr<Sprite>> uis_;
	Score score_;
	Rank rank_;
	Vector2 scorePos_ = { 1000.0f,250.0f };
	Vector2 rankPos_ = { 808.0f,186.0f };

	std::array<uint32_t, 2> playerTex_;
	std::array<uint32_t, 2> playerRunTex_;
	std::array<std::unique_ptr<Object2d>, 2> player_;

	int32_t animationTime_ = 0;
	int32_t changeFrame_ = 5;

	int32_t animationNum_ = 0;

	uint32_t saunaRoomTex_;
	std::vector<std::unique_ptr<Object2d>> saunaRooms_;

	uint32_t preStageNum_ = 0;

	float param_ = 0.0f;

	float moveStartPos_ = 0.0f;
	float moveEndPos_ = 0.0f;

	struct WorkFloating {
		float param_ = 0.0f;
		//振幅
		float amplitude_ = 1.5f;
		//サイクル<frame>
		int32_t cycle_ = 60;
	};

	WorkFloating arrowFloating_;

	Vector2 LArrowPos_{};
	Vector2 RArrowPos_{};

private:

	enum Charactor {
		Blue,
		Orange,
	};

	enum class Mode {
		Root,
		Move,
		Enter,
	};

private:

	void RootInit();
	void RootUpdate();

	void MoveInit();
	void MoveUpdate();

	void EnterInit();
	void EnterUpdate();

private:

	Mode mode_ = Mode::Root;
	std::optional<Mode> modeRequest_ = Mode::Root;

};
