#pragma once
#include "Vec2.h"
#include "Sprite.h"
#include "Camera.h"
#include <memory>
#include <list>
#include <random>

class TransitionEffect {
public:

	TransitionEffect() : randomEngine(seedGenerator()){}

	void Init();

	void Update();

	void InEffectUpdate();

	void OutEffectUpdate();

	void Draw();
	//演出開始
	void InEffectStart();
	//演出終了
	void OutEffectStart();

	void IsActiveOn();

	void IsActiveOff();

	void FlagUpdate();

	bool IsActive() const { return isActive_; }

	//演出開始時
	bool isActiveStart() const { return !preIsActive_ && isActive_; }
	//演出終了時
	bool isActiveEnd() const { return preIsActive_ && !isActive_; }
	//イン演出開始時
	bool InEffectEnter() const { return !preIsInEffect_ && isInEffect_; }
	//イン演出終了時
	bool InEffectEnd() const { return preIsInEffect_ && !isInEffect_; }
	//アウト演出開始時
	bool OutEffectEnter() const { return !preIsOutEffect_ && isOutEffect_; }
	//アウト演出終了時
	bool OutEffectEnd() const { return preIsOutEffect_ && !isOutEffect_; }

private:

	Camera camera_;

	std::unique_ptr<Sprite> effect_;

	const float deltaTime_ = 1.0f / 60.0f;

	std::random_device seedGenerator;
	std::mt19937 randomEngine;

	//演出中か
	bool isActive_ = false;
	bool preIsActive_ = false;

	bool isInEffect_ = false;
	bool preIsInEffect_ = false;

	bool isOutEffect_ = false;
	bool preIsOutEffect_ = false;

	float param_ = 0.0f;

	float alpha_ = 0.0f;

};

