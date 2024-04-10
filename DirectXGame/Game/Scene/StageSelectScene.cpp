#include "StageSelectScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>

StageSelectScene::~StageSelectScene() {}

void StageSelectScene::Init() {

	bgTexture_ = TextureManager::Load("uvChecker.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

}

void StageSelectScene::Update() {
	DebugGUI();

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

}

void StageSelectScene::DrawBackGround() {

	backGround_->Draw();

}

void StageSelectScene::DrawObject() {



}

void StageSelectScene::DrawParticle() {



}

void StageSelectScene::DrawUI() {



}

void StageSelectScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}


