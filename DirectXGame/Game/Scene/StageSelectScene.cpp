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

	bgTexture_ = TextureManager::Load("backGround/stageSelectBG.png");
	stageNumTex_ = TextureManager::Load("stageNumber.png");

	backGround_.reset(Sprite::Create(bgTexture_, { 640.0f,360.0f }));

	stageNum_.reset(Sprite::Create(stageNumTex_, { 640.0f,400.0f }));
	stageNum_->SetSize({ 96.0f, 96.0f });
	stageNum_->SetTextureArea({ 0.0f,0.0f }, { 32.0f,32.0f });

}

void StageSelectScene::Update() {
	DebugGUI();

	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		SceneManager::GetInstance()->ChangeScene("Game");
	}

	if (Input::GetInstance()->TriggerLStick(Input::Stick::Right)) {
		stageNumber_++;
	}else if (Input::GetInstance()->TriggerLStick(Input::Stick::Left)) {
		stageNumber_--;
	}

#ifdef _DEBUG

	if (Input::GetInstance()->TriggerKey(DIK_RIGHT)) {
		stageNumber_++;
	}
	else if (Input::GetInstance()->TriggerKey(DIK_LEFT)) {
		stageNumber_--;
	}

#endif // _DEBUG


	stageNumber_= max(1, min(stageNumber_, 9));

	stageNum_->SetTextureArea({ 32.0f * stageNumber_, 32.0f * stageNumber_ }, { 32.0f,32.0f });
}

void StageSelectScene::DrawBackGround() {

	backGround_->Draw();

}

void StageSelectScene::DrawObject() {



}

void StageSelectScene::DrawParticle() {



}

void StageSelectScene::DrawUI() {

	stageNum_->Draw();

}

void StageSelectScene::DebugGUI() {
#ifdef _DEBUG



#endif // _DEBUG
}


