#include "TransitionEffect.h"
#include "TextureManager.h"
#include "Easing.h"

void TransitionEffect::Init() {

	camera_.Init();

	uint32_t tex_ = TextureManager::Load("white.png");
	effect_.reset(Sprite::Create(tex_, { 640.0f,360.0f }));
	effect_->SetSize({ 1280.0f,720.0f });

}

void TransitionEffect::Update() {

	if (isInEffect_) {

		InEffectUpdate();

	}else if (isOutEffect_) {

		OutEffectUpdate();
	}

	effect_->SetColor({ 0.0f,0.0f,0.0f,alpha_ });
	camera_.UpdateViewMatrix();
}

void TransitionEffect::InEffectUpdate() {

	param_ += 0.05f;
	param_ = std::clamp(param_, 0.0f, 1.0f);
	float T = Easing::easeInSine(param_);
	alpha_ = Lerp(T, 1.0f, 0.0f);
	if (param_ >= 1.0f) {
		IsActiveOff();
	}

	/*if (smokeDatas_.empty()) {
		InEffectStart();
	}*/

}

void TransitionEffect::OutEffectUpdate() {

	param_ += 0.02f;
	param_ = std::clamp(param_, 0.0f, 1.0f);
	float T = Easing::easeInSine(param_);
	alpha_ = Lerp(T, 0.0f, 1.0f);
	if (param_ >= 1.0f) {
		InEffectStart();
	}

	//param_ += deltaTime_;

	/*if (param_ >= 8.0f) {
		InEffectStart();
	}*/

	/*smokeEmitter_.frequencyTime_ += deltaTime_;
	if (smokeEmitter_.frequencyTime_ >= smokeEmitter_.frequency_) {
		smokeDatas_.splice(smokeDatas_.end(), Emit(smokeEmitter_, randomEngine));
		smokeEmitter_.frequencyTime_ = 0.0f;
	}*/

}

void TransitionEffect::Draw() {

	
	effect_->Draw();
	/*Particle::preDraw();
	smoke_->Draw(smokeDatas_, camera_);
	Particle::postDraw();*/
}

void TransitionEffect::InEffectStart() {
	isInEffect_ = true;
	isOutEffect_ = false;
	param_ = 0.0f;
}

void TransitionEffect::OutEffectStart() {

	isInEffect_ = false;
	isOutEffect_ = true;
	param_ = 0.0f;
}

void TransitionEffect::IsActiveOn() {
	isActive_ = true;
}

void TransitionEffect::IsActiveOff() {
	isActive_ = false;
	isOutEffect_ = false;
	isInEffect_ = false;
}

void TransitionEffect::FlagUpdate() {
	preIsActive_ = isActive_;
	preIsInEffect_ = isInEffect_;
	preIsOutEffect_ = isOutEffect_;
}
