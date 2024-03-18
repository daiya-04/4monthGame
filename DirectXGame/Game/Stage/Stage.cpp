#include "Stage.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>

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
	for (auto& block : map_) {
		block->Update();
	}

	//サウナストーン更新
	for (auto& stone : stones_) {
		stone->Update();
	}

	/*for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			map_[y][x]->Update();
		}

	}*/

}

void Stage::Draw(const Camera& camera) {

	//サウナストーン描画
	for (auto& stone : stones_) {
		
		if (stone->GetPosition().x >= camera.translation_.x - Block::kBlockSize_ &&
			stone->GetPosition().x <= camera.translation_.x + 1280 + Block::kBlockSize_ &&
			stone->GetPosition().y >= camera.translation_.y - Block::kBlockSize_ &&
			stone->GetPosition().y <= camera.translation_.y + 720 + Block::kBlockSize_) {
			stone->Draw(camera);
		}

	}

	//ブロックの描画
	for (auto& block : map_) {

		if (block->GetPosition().x >= camera.translation_.x - Block::kBlockSize_ &&
			block->GetPosition().x <= camera.translation_.x + 1280 + Block::kBlockSize_ &&
			block->GetPosition().y >= camera.translation_.y - Block::kBlockSize_ &&
			block->GetPosition().y <= camera.translation_.y + 720 + Block::kBlockSize_) {
			block->Draw(camera);
		}

	}
	
	/*for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			map_[y][x]->Draw();
		}

	}*/

}

void Stage::Load(uint32_t stageNumber) {

	std::string fileName = "./Resources/Maps/stage";

	fileName += std::to_string(stageNumber);

	fileName += ".csv";

	std::ifstream newFile(fileName);

	//ファイル読み込みが出来なかったら処理を止める
	if (newFile.fail()) {
		MessageBox(nullptr, L"ファイルを読み込めませんでした。", L"Stage - Load", 0);
		return;
	}

	//保険
	assert(!newFile.fail());

	//一行分の文字を格納する文字列
	std::string line;

	//ブロックを生成、初期化
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {
		
		//一行取得
		std::getline(newFile, line);

		//カンマ区切りで読み込む用の文字列
		std::istringstream iss(line);

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			//格納する数字
			std::string sNum;
			//数字を格納
			std::getline(iss, sNum, ',');
			//番号からTypeを選ぶ
			uint32_t num = std::stoi(sNum);
			Block::BlockType type;
			//範囲外のものを選ばないように防ぐ
			if (num < Block::BlockType::kMaxBlock) {
				type = static_cast<Block::BlockType>(num);
			}
			else {
				num = 0;
				type = static_cast<Block::BlockType>(num);
			}

			//空気ブロックでなければブロックを生成
			if (num != 0) {

				//ブロックの実体を生成、初期化
				std::shared_ptr<Block> block = std::make_shared<Block>();
				block->Initialize({ x * float(Block::kBlockSize_), y * float(Block::kBlockSize_) }, type);
				block->SetPlayer(player_);
				map_.push_back(block);

				/*map_[y][x] = std::make_shared<Block>();
				map_[y][x]->Initialize({ x * 32.0f, y * 32.0f }, type);*/

			}

		}

	}

	/*{

		//サウナストーンの数
		uint32_t saunaStoneNumber = 0;
		//カンマ区切りで数を取得
		std::getline(newFile, line, ',');
		if (!line.empty()) {
			saunaStoneNumber = std::stoi(line);
		}

		//サウナストーンリセット
		stones_.clear();

		//サウナストーンを生成、
		for (uint32_t i = 0; i < saunaStoneNumber; i++) {

			Vector2 position{};

			//一行取得
			std::getline(newFile, line, ',');

			//カンマ区切りで読み込む用の文字列
			std::istringstream iss(line);

			std::string sNum;

			//座標を格納
			std::getline(iss, sNum, ',');
			position.x = std::stof(sNum);
			std::getline(iss, sNum, ',');
			position.y = std::stof(sNum);

			std::shared_ptr<SaunaStone> stone = std::make_shared<SaunaStone>();
			stone->Initialize(position);
			stones_.push_back(stone);

		}

	}*/ 


}
