#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
#include <vector>
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

	uint32_t saunaRoomTex_;
	std::vector<std::unique_ptr<Object2d>> saunaRooms_;

	uint32_t preStageNum_ = 0;

	float param_ = 0.0f;

	float moveStartPos_{};
	float moveEndPos_{};

private:

	enum class Mode {
		Root,
		Move,
	};

private:

	void RootInit();

	void RootUpdate();

	void MoveInit();

	void MoveUpdate();

private:

	Mode mode_ = Mode::Root;
	std::optional<Mode> modeRequest_ = Mode::Root;

};
