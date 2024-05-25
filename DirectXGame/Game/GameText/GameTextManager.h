#pragma once
#include <stdint.h>
#include <memory>
#include <array>
#include "Text/Text.h"
#include "Object2dInstancing.h"
#include "Sprite.h"
#include "Vec2.h"
class GameTextManager
{
public:

	struct NineSliceData
	{
		Vector2 position;//中心座標
		Vector2 size;//全体サイズ
		Vector2 partSize = {5.0f,5.0f};//小さいパーツのサイズ

		Vector2 textureSize = { 16.0f,16.0f };//テクスチャのサイズ
		Vector2 texturePartSize = {5.0f,5.0f};//テクスチャの小さいパーツのサイズ
	};

	static GameTextManager* GetInstance() {
		static GameTextManager instance;
		return &instance;
	};

	//gameSceneのInitializeで呼び出す
	//void InitializeStage(uint32_t stageNum);

	~GameTextManager() {};

	//dataからInstancingObjectに情報を積める
	void SetNineSliceData();

	//テストでimguiから値を操作する
	void TestUpdate();

	void Draw();

private:

	void Initialize();

	GameTextManager() { Initialize(); };

	uint32_t stageNumber_ = 1;

	NineSliceData nineSliceData_;
	//scale1が1のときのサイズ
	Vector2 textBoxOriginSize_ = {1000.0f,200.0f};

	std::array<std::unique_ptr<Sprite>,9> nineSliceTextureBox_;

};