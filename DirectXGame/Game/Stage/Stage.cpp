#include "Stage.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize(uint32_t stageNumber) {

	//マップロード
	Load(stageNumber);

}

void Stage::Update() {

	//ブロックの更新
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			map_[y][x]->Update();
		}

	}

}

void Stage::Draw() {

	//ブロックの描画
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			map_[y][x]->Draw();
		}

	}

}

void Stage::Load(uint32_t stageNumber) {


	//ブロックを生成、初期化
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

		}

	}

}
