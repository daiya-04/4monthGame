#include "TitleScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "SceneManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>

TitleScene::~TitleScene() {}

void TitleScene::Init() {

	bgTexture_ = TextureManager::Load("backGround/titleBG.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

}

void TitleScene::Update() {
	DebugGUI();

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		SceneManager::GetInstance()->ChangeScene("StageSelect");
	}


}

void TitleScene::DrawBackGround() {

	backGround_->Draw();

}

void TitleScene::DrawObject() {



}

void TitleScene::DrawParticle() {



}

void TitleScene::DrawUI() {



}

void TitleScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}


