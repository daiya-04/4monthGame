#include "DefaultScene.h"

uint32_t DefaultScene::stageNumber_ = 1;
std::unique_ptr<Stage> DefaultScene::stage_ = nullptr;
std::shared_ptr<Player> DefaultScene::player_ = nullptr;
bool DefaultScene::isSceneChange_ = false;