#include "Framework.h"

#include "ImGuiManager.h"
#include "Input.h"
#include "TextureManager.h"
#include "ModelManager.h"
#include "AudioManager.h"
#include "DirectionalLight.h"
#include "Sprite.h"
#include "Object2d.h"
#include "Object2dInstancing.h"
#include "Particle.h"
#include "GlobalVariables.h"
#include "Audio.h"


void DSFramework::Init(){
	
	WinApp::GetInstance()->CreateGameWindow(L"～互サ道～");
#ifdef NDEBUG
	WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kFullScreen);
#endif // _DEBUG
	
	DirectXCommon::GetInstance()->Initialize();
	ImGuiManager::GetInstance()->Initialize();
	
	Input::GetInstance()->Initialize();
	AudioManager::GetInstance()->Init();

	TextureManager::GetInstance()->Initialize();
	Model::SetDevice();
	//TextureManager::Load("uvChecker.png");

	Sprite::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), WinApp::kClientWidth, WinApp::kClientHeight);
	Object2d::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), WinApp::kClientWidth, WinApp::kClientHeight);
	Object2dInstancing::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), WinApp::kClientWidth, WinApp::kClientHeight);
	Particle::StaticInitialize(DirectXCommon::GetInstance()->GetDevice(), DirectXCommon::GetInstance()->GetCommandList());

	GlobalVariables::GetInstance()->LoadFiles();

	DirectionalLight::GetInstance()->Init();

	SceneManager::GetInstance()->Init();

}

void DSFramework::Update(){

	if (WinApp::GetInstance()->ProcessMessage()) {
		//AudioManager::GetInstance()->AllAudioStop();
		WinApp::GetInstance()->GameEnd(); 
	}

	/*if ((Input::GetInstance()->PushKey(DIK_LCONTROL) || Input::GetInstance()->PushKey(DIK_RCONTROL)) && Input::GetInstance()->TriggerKey(DIK_9)) {
		WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kFullScreen);
	}
	if ((Input::GetInstance()->PushKey(DIK_LCONTROL) || Input::GetInstance()->PushKey(DIK_RCONTROL)) && Input::GetInstance()->TriggerKey(DIK_0)) {
		WinApp::GetInstance()->ChangeScreenMode(WinApp::ScreenMode::kWindow);
	}*/

	ImGuiManager::GetInstance()->Begin();

	Input::GetInstance()->Update();
	AudioManager::GetInstance()->Update();

	GlobalVariables::GetInstance()->Update();

	DirectionalLight::GetInstance()->Update();

	SceneManager::GetInstance()->Update();

}

void DSFramework::Finalize(){

	ImGuiManager::GetInstance()->Finalize();
	WinApp::GetInstance()->TerminateGameWindow();

}

void DSFramework::Run(){

	//初期化
	Init();

	//ゲームループ
	while (true) {

		

		//更新
		Update();

		//終了リクエストが来たら抜ける
		if (WinApp::GetInstance()->IsEndRequest()) {
			AudioManager::GetInstance()->AllAudioStop();
			if (AudioManager::GetInstance()->IsAllAudioStop()) {
				break;
			}
		}

		//描画
		Draw();

	}

	//ゲームの終了
	Finalize();
}
