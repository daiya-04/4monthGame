#include "GameScene.h"
#include "DirectXCommon.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>

GameScene::~GameScene() {}

void GameScene::Init(){
	commandList_ = DirectXCommon::GetInstance()->GetCommandList();
	prevScene.reset( new PostEffect());
	prevScene->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");
	
	nextScene.reset(new PostEffect());
	nextScene->Init(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	dualSceneDrawer_.reset(new DualSceneDrawer());
	dualSceneDrawer_->Init();

	weightCircle_.reset(new WeightCircle);
	weightCircle_->Init();

	sample0.reset(new Sprite(TextureManager::GetInstance()->Load("sample0.png"), {640.0f,360.0f},10.0f));
	sample0->Initialize();
	sample1.reset(new Sprite(TextureManager::GetInstance()->Load("sample1.png"), { 640.0f,360.0f }, 10.0f));
	sample1->Initialize();
}

void GameScene::Update(){
	DebugGUI();

	weightCircle_->Update();
}

void GameScene::DrawNotSetPipeline() {
	prevScene->PreDrawScene(commandList_);
	//一個目のscene描画
	Sprite::preDraw(commandList_);
	sample0->Draw();
	Sprite::postDraw();
	prevScene->PostDrawScene(commandList_);

	nextScene->PreDrawScene(commandList_);
	//二個目のscene描画
	Sprite::preDraw(commandList_);
	sample1->Draw();
	Sprite::postDraw();
	nextScene->PostDrawScene(commandList_);

	weightCircle_->Draw();

	//DirectXCommon::GetInstance()->preDraw();
	

}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawModel(){

	

}

void GameScene::DrawParticleModel(){



}

void GameScene::DrawParticle(){

	

}

void GameScene::DrawUI(){
	
	dualSceneDrawer_->Draw(commandList_, prevScene->GetSrvHandleGPU(), nextScene->GetSrvHandleGPU(), weightCircle_->GetHandle());

}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	

#endif // _DEBUG
}


