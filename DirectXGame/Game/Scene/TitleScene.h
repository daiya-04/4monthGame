#pragma once
#include "DefaultScene.h"
#include <memory>
#include <list>
#include <unordered_map>
#include <vector>
#include <optional>

#include "Sprite.h"
#include "Object2d.h"
#include "Particle.h"
#include "Camera.h"
#include "WorldTransform.h"
#include "PointLight.h"
#include "SpotLight.h"

#include "Option.h"
#include "TitleAnimation.h"


class TitleScene : public DefaultScene {
public:

	void Init()override;

	void Update()override;

	void DrawBackGround()override;

	void DrawObject()override;

	void DrawParticle()override;

	void DrawNotSetPipeline()override {};

	void DrawUI()override;

	void DebugGUI()override;

	~TitleScene()override;

	void LoadData();

	void ApplyGlobalVariables();

	void FloatingGimmickInit();

	void FloatingGimmickUpdate();

	void ButtonEffectInit();

	void ButtonEffectUpdate();

	void BoundingInit(const Vector2& axisPos);

	void BoundingUpdate(Sprite* sprite);

private:

	const std::string dataName = "TitleUI";

	char filePathBuff[256]{};
	char groupNameBuff[256]{};
	Vector2 addPos{};

	uint32_t bgTexture_;
	std::unique_ptr<Sprite> backGround_;

	std::unordered_map<std::string, std::unique_ptr<Sprite>> uis_;

	uint32_t circleTex_ = 0;
	std::unique_ptr<Sprite> circle_;

	std::unique_ptr<Option> option_;

	std::unique_ptr<TitleAnimation> titleAnima_;

	struct WorkFloating {
		float param_ = 0.0f;
		//振幅
		float amplitude_ = 1.0f;
		//サイクル<frame>
		int32_t cycle_ = 60;
		//軸座標
		Vector2 axisPos_{};

		Vector2 addPos_{};
	};

	struct WorkButtonEffect {
		float param_ = 0.0f;
		float alpha_ = 1.0f;
		float scale_ = 1.0f;
	};

	struct WorkBounding {
		Vector2 velocity_{};
		Vector2 accel_{};
		Vector2 axisPos_{};
		Vector2 addPos_{};
	};

	WorkFloating workFloating_;
	WorkButtonEffect buttonEffect_;
	WorkBounding workBounding_;

	bool isOpening_ = true;

	Audio* selectSE_;
	Audio* moveSE_;
	Audio* cancelSE_;
	Audio* titleBGM_;

private:

	enum class Select {
		Start,
		Option,
		Exit,
	};

	Select select_ = Select::Start;
	std::optional<Select> selectRequest_ = Select::Start;

private:

	void StartInit();
	void StartUpdate();

	void OptionInit();
	void OptionUpdate();

	void ExitInit();
	void ExitUpdate();
	

};
