#pragma once
#include "IScene.h"
#include <stdint.h>

/// <summary>
/// シーン間共通の変数などを持つシーン。ここから他のシーンに継承
/// </summary>
class DefaultScene : public IScene {
public:


protected:

	static const uint32_t kMaxStage_ = 1;

	static uint32_t stageNumber_;

};

