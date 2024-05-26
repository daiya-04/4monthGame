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

	enum Phase {
		OPEN,
		VIEW,
		CLOSE,
		END
	};

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
	void InitializeStage(uint32_t stageNum);

	~GameTextManager() {};

	//dataからInstancingObjectに情報を積める
	void SetNineSliceData();

	//テストでimguiから値を操作する
	void TestUpdate();

	void Update();

	void Draw();

	bool GetIsEnd() { return isEnd_; };

private:

	void AppryGlobalVariables();

	//各フェーズ処理
	void Open();
	void View();
	void Close();
	

	void Initialize();

	GameTextManager() { Initialize(); };

	uint32_t stageNumber_ = 1;

	NineSliceData nineSliceData_;
	//scale1が1のときのサイズ
	Vector2 textBoxOriginSize_ = {1000.0f,200.0f};
	Vector2 nameBackSize_ = {0,0};
	Vector2 nameBackPosition_ = {0,0};


	std::array<std::unique_ptr<Sprite>,9> nineSliceTextureBox_;

	std::unique_ptr<Sprite> nameBack_;//名前ひょうじの背景

	std::unique_ptr<Text> mainText_;//本文

	std::unique_ptr<Text> next_;//次へボタン

	//std::wstring testText = L"";

	std::vector<std::wstring> textList_;//現在ステージで表示するテキストのリスト
	size_t listIndex_;
	Phase phase_;

	float parametric_;//イージング用媒介変数

	bool isEnd_;//終了したか

	const std::string dataName = "GameText";
};