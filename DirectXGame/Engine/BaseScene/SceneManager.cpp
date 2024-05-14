#include "SceneManager.h"
#include "SceneFactory.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "Object2d.h"
#include "Particle.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include "Input.h"
#include <cassert>

SceneManager* SceneManager::GetInstance(){
	static SceneManager instance;

	return &instance;
}

void SceneManager::Init() {

	sceneFactory_ = std::make_unique<SceneFactory>();

	//uint32_t whiteTex = TextureManager::Load("white.png");

	postEffect_ = std::make_unique<PostEffect>();
	postEffect_->Init();

	transitionEffect_ = std::make_unique<TransitionEffect>();
	transitionEffect_->Init();

}

void SceneManager::Update(){

#ifdef _DEBUG

	ImGui::Begin("window");

	ImGui::Text("Frame rate: %6.2f fps", ImGui::GetIO().Framerate);

	ImGui::End();

	
#endif // _DEBUG

	if (isStart_) {
		scene_ = std::move(nextScene_);
		scene_->Init();
		transitionEffect_->IsActiveOff();
		isStart_ = false;
	}
	
	//シーンの更新
	if (nextScene_ == nullptr && !transitionEffect_->IsActive()) {
		scene_->Update();
	}

	//シーンの切り替えがないならここで終了
	

	if (transitionEffect_->isActiveStart()) {
		transitionEffect_->OutEffectStart();
	}

	transitionEffect_->Update();

	if (transitionEffect_->InEffectEnter()) {
		scene_ = std::move(nextScene_);
		scene_->Init();
		scene_->Update();
	}

	transitionEffect_->FlagUpdate();
}

void SceneManager::Draw(ID3D12GraphicsCommandList* commandList){

	scene_->DrawNotSetPipeline();

	postEffect_->PreDrawScene(commandList);

	

	postEffect_->PostDrawScene(commandList);

	DirectXCommon::GetInstance()->preDraw();

	///背景スプライト
	Sprite::preDraw(commandList);

	scene_->DrawBackGround();

	Sprite::postDraw();

	DirectXCommon::GetInstance()->ClearDepthBaffer();

	///2dオブジェクト
	Object2d::preDraw(commandList);

	scene_->DrawObject();

	Object2d::postDraw();

	///パーティクル
	Particle::preDraw();

	scene_->DrawParticle();

	Particle::postDraw();

	postEffect_->Draw(commandList);

	///UI
	Sprite::preDraw(commandList);

	scene_->DrawUI();

	Sprite::postDraw();

	if (transitionEffect_->IsActive()) {
		transitionEffect_->Draw();
	}

	ImGuiManager::GetInstance()->Draw();

	DirectXCommon::GetInstance()->postDraw();
	

}

void SceneManager::ChangeScene(const std::string& sceneName){

	assert(nextScene_ == nullptr);

	nextScene_ = sceneFactory_->CreateScene(sceneName);
	transitionEffect_->IsActiveOn();
}


