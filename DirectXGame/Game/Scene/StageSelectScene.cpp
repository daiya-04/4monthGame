#include "StageSelectScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>

StageSelectScene::~StageSelectScene() {}

void StageSelectScene::Init() {



}

void StageSelectScene::Update() {
	DebugGUI();

	if (Input::GetInstance()->TriggerKey(DIK_E)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

}

void StageSelectScene::DrawBackGround() {



}

void StageSelectScene::DrawObject() {



}

void StageSelectScene::DrawParticleModel() {



}

void StageSelectScene::DrawParticle() {



}

void StageSelectScene::DrawUI() {



}

void StageSelectScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}


