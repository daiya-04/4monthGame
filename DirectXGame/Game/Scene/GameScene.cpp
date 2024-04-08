#include "GameScene.h"

#include "TextureManager.h"
#include "ModelManager.h"
#include "ImGuiManager.h"
#include "Audio.h"
#include "Input.h"
#include <random>

GameScene::~GameScene() {}

void GameScene::Init(){

	camera_.Init();

	uint32_t circleTex = TextureManager::Load("uvChecker.png");

	particle_.reset(Particle::Create(circleTex, 10000));

	emitter_ = {
		{640.0f,360.0f},
		//{0.0f,0.0f},
		200,
		0.2f,
		0.0f
	};
	/*std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	
	data_.splice(data_.end(), Particle::Emit(emitter_, randomEngine));*/
}

void GameScene::Update(){
	DebugGUI();

	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	const float kDeltaTime = 1.0f / 60.0f;

	emitter_.frequencyTime_ += kDeltaTime;
	if (emitter_.frequencyTime_ >= emitter_.frequency_) {
		data_.splice(data_.end(), Particle::Emit(emitter_, randomEngine));
		emitter_.frequencyTime_ = 0.0f;
	}

	for (std::list<Particle::ParticleData>::iterator itParticle = data_.begin(); itParticle != data_.end(); itParticle++) {
		(*itParticle).pos_ += (*itParticle).velocity_;
		(*itParticle).currentTime_ += kDeltaTime;
	}
	
	camera_.UpdateViewMatrix();
}

void GameScene::DrawBackGround(){

	

}

void GameScene::DrawObject(){

	

}

void GameScene::DrawParticleModel(){



}

void GameScene::DrawParticle(){

	particle_->Draw(data_, camera_);

}

void GameScene::DrawUI(){

	

}

void GameScene::DebugGUI(){
#ifdef _DEBUG

	

#endif // _DEBUG
}


