#include "SceneFactory.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "StageSelectScene.h"

std::unique_ptr<IScene> SceneFactory::CreateScene(const std::string& sceneName){

    std::unique_ptr<IScene> newScene = nullptr;

    if (sceneName == "Title") {
        newScene = std::make_unique<TitleScene>();
    }
    else if (sceneName == "StageSelect") {
        newScene = std::make_unique<StageSelectScene>();
    }
    else if (sceneName == "Game") {
        newScene = std::make_unique<GameScene>();
    }

    return newScene;
}
