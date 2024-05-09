#pragma once
#include "IScene.h"
#include <stdint.h>
#include "SceneManager.h"
#include "Stage/Stage.h"
#include "Player/Player.h"

/// <summary>
/// シーン間共通の変数などを持つシーン。ここから他のシーンに継承
/// </summary>
class DefaultScene : public IScene {
public:

	static const uint32_t kMaxStage_ = 10;

protected:

	static bool isSceneChange_;

	static uint32_t stageNumber_;

	static std::shared_ptr<Player> player_;

	static std::unique_ptr<Stage> stage_;

};

