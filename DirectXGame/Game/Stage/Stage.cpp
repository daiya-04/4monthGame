#include "Stage.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Game/Player/Player.h"

Stage::Stage()
{
}

Stage::~Stage()
{
}

void Stage::Initialize() {

	/*map_.clear();*/
	stones_.clear();

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			blockPositions_[y][x] = 0;

		}

	}

	CreateEntity();

}

void Stage::Update() {

	/*map_.remove_if([&](auto& block) {

		if (block->GetIsBreak()) {
			blockPositions_[block->GetBlockPositionY()][block->GetBlockPositionX()] = 0;
			return true;
		}

		return false;

	});*/

	if (Input::GetInstance()->TriggerKey(DIK_1)) {
		CreateIceBlock();
	}

	if (Input::GetInstance()->TriggerKey(DIK_2)) {
		BreakIceBlock();
	}

	if (Input::GetInstance()->TriggerKey(DIK_3)) {
		SwitchBlock();
	}

	if (Input::GetInstance()->TriggerKey(DIK_4)) {
		BreakAllBlock();
	}

	//ブロックの更新
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			blockPositions_[y][x] = map_[y][x]->GetType();
			SetUV(map_[y][x].get());
			map_[y][x]->Update();
		}

	}

	/*for (auto& block : map_) {

		SetUV(block.get());

		block->Update();

	}*/

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

void Stage::Draw() {

	//サウナストーン描画
	for (auto& stone : stones_) {
		
		if (stone->GetPosition().x >= camera_->translation_.x - Block::kBlockSize_ &&
			stone->GetPosition().x <= camera_->translation_.x + 1280 + Block::kBlockSize_ &&
			stone->GetPosition().y >= camera_->translation_.y - Block::kBlockSize_ &&
			stone->GetPosition().y <= camera_->translation_.y + 720 + Block::kBlockSize_) {
			stone->Draw(*camera_);
		}

	}

	//ブロックの描画
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			if (map_[y][x]->GetPosition().x >= camera_->translation_.x - Block::kBlockSize_ &&
				map_[y][x]->GetPosition().x <= camera_->translation_.x + 1280 + Block::kBlockSize_ &&
				map_[y][x]->GetPosition().y >= camera_->translation_.y - Block::kBlockSize_ &&
				map_[y][x]->GetPosition().y <= camera_->translation_.y + 720 + Block::kBlockSize_) {
				map_[y][x]->Draw(*camera_);
			}
			
		}

	}

	/*for (auto& block : map_) {

		if (block->GetPosition().x >= camera_->translation_.x - Block::kBlockSize_ &&
			block->GetPosition().x <= camera_->translation_.x + 1280 + Block::kBlockSize_ &&
			block->GetPosition().y >= camera_->translation_.y - Block::kBlockSize_ &&
			block->GetPosition().y <= camera_->translation_.y + 720 + Block::kBlockSize_) {
			block->Draw(*camera_);
		}

	}*/
	
	/*for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			map_[y][x]->Draw();
		}

	}*/

}

void Stage::SwitchBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			
			if (map_[y][x]->GetType() == Block::BlockType::kSnow) {
				map_[y][x]->ChangeType(Block::BlockType::kMagma);
			}
			else if (map_[y][x]->GetType() == Block::BlockType::kMagma) {
				map_[y][x]->ChangeType(Block::BlockType::kSnow);
			}

			blockPositions_[y][x] = map_[y][x]->GetType();

		}

	}

	/*for (auto& block : map_) {

		if (block->GetType() == Block::BlockType::kSnow) {
			block->ChangeType(Block::BlockType::kMagma);
			SetUV(block.get());
		}
		else if (block->GetType() == Block::BlockType::kMagma) {
			block->ChangeType(Block::BlockType::kSnow);
			SetUV(block.get());
		}

	}*/

}

void Stage::CreateIceBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		
		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			
			//ブロックが無い且つプレイヤーと当たらない部分に氷ブロックを生成
			if (map_[y][x]->GetIsBreak() && !IsCollision(map_[y][x]->GetCollision(), player_->GetCollision())) {

				map_[y][x]->ChangeType(Block::BlockType::kIceBlock);
				map_[y][x]->Repair();

				blockPositions_[y][x] = BaseBlock::BlockType::kIceBlock;

			}

		}

	}

}

void Stage::BreakIceBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			//氷ブロックを破壊する
			if (map_[y][x]->GetType() == Block::BlockType::kIceBlock) {

				map_[y][x]->Break();

			}

		}

	}

	/*for (auto& block : map_) {

		if (block->GetType() == BaseBlock::BlockType::kIceBlock) {
			block->Break();
		}

	}*/

}

void Stage::BreakAllBlock() {

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			//壊せないブロック以外を破壊する
			if (Block::CheckCanBreak(map_[y][x]->GetType())) {

				map_[y][x]->Break();

			}

		}

	}

	/*for (auto& block : map_) {

		if (block->GetType() > BaseBlock::BlockType::kUnbreakable) {
			block->Break();
		}

	}*/

}

void Stage::CreateEntity() {

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			//実体がない場合、生成する
			if (!map_[y][x]) {

				//ブロックの実体を生成、初期化
				map_[y][x] = std::make_shared<Block>();
				map_[y][x]->Initialize({ x * float(Block::kBlockSize_), y * float(Block::kBlockSize_) }, Block::BlockType::kNone);
				map_[y][x]->SetPlayer(player_);
				map_[y][x]->SetBlockPosition(x, y);

			}

		}

	}

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
			//ブロックのパラメータ変更
			map_[y][x]->ChangeType(type);

			blockPositions_[y][x] = num;

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

void Stage::SetUV(Block* block) {

	int32_t px = block->GetBlockPositionX();
	int32_t py = block->GetBlockPositionY();

	//周囲八マスの判定を取るための変数
	uint32_t left = std::clamp(px - 1, 0, int(kMaxStageWidth_));
	uint32_t right = std::clamp(px + 1, 0, int(kMaxStageWidth_));
	uint32_t top = std::clamp(py - 1, 0, int(kMaxStageHeight_));
	uint32_t bottom = std::clamp(py + 1, 0, int(kMaxStageHeight_));
	
	//周囲八マスと現在のブロックの数字
	int32_t centerNum = blockPositions_[py][px];
	int32_t leftNum = blockPositions_[py][left];
	int32_t rightNum = blockPositions_[py][right];
	int32_t topNum = blockPositions_[top][px];
	int32_t bottomNum = blockPositions_[bottom][px];
	int32_t topLeftNum = blockPositions_[top][left];
	int32_t topRightNum = blockPositions_[top][right];
	int32_t bottomLeftNum = blockPositions_[bottom][left];
	int32_t bottomRightNum = blockPositions_[bottom][right];

	//周囲八マスに同一ブロックがある場合
	if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(5, 0);
	}

	//左上以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(5, 4);
	}

	//右上以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(6, 4);
	}

	//左下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(4, 4);
	}

	//右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(7, 4);
	}

	//左上、左下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(0, 4);
	}

	//左上、右上以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(1, 4);
	}

	//左上、右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(6, 3);
	}

	//右上、左下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(5, 3);
	}

	//右上、右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(2, 4);
	}

	//左下、右下以外にブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(3, 4);
	}

	//左上のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum &&
		centerNum != bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(3, 3);
	}

	//右上のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum &&
		centerNum != bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(4, 3);
	}

	//左下のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum != topRightNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
		block->SetUVPosition(2, 3);
	}

	//右下のみブロックあり
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum != topRightNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
		block->SetUVPosition(1, 3);
	}

	//左以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topRightNum && centerNum == bottomRightNum) {
			block->SetUVPosition(1, 0);
	}

	//右以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum == bottomLeftNum) {
			block->SetUVPosition(3, 0);
	}

	//上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomLeftNum && centerNum == bottomRightNum) {
			block->SetUVPosition(2, 0);
	}

	//下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topLeftNum && centerNum == topRightNum) {
			block->SetUVPosition(4, 0);
	}

	//左と上以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomRightNum) {
			block->SetUVPosition(0, 1);
	}

	//右と上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomLeftNum) {
			block->SetUVPosition(1, 1);
	}

	//左と下以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topRightNum) {
			block->SetUVPosition(3, 1);
	}

	//右と下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topLeftNum) {
			block->SetUVPosition(2, 1);
	}

	//左と右上以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topRightNum && centerNum == bottomRightNum) {
			block->SetUVPosition(0, 6);
	}

	//左と右下以外ブロックあり(角は問わない)
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topRightNum && centerNum != bottomRightNum) {
			block->SetUVPosition(1, 6);
	}

	//右と左上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum != topLeftNum && centerNum == bottomLeftNum) {
			block->SetUVPosition(4, 6);
	}

	//右と左下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum &&
		centerNum == topLeftNum && centerNum != bottomLeftNum) {
			block->SetUVPosition(5, 6);
	}

	//上と左下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum != bottomLeftNum && centerNum == bottomRightNum) {
			block->SetUVPosition(2, 6);
	}

	//上と右下以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum &&
		centerNum == bottomLeftNum && centerNum != bottomRightNum) {
			block->SetUVPosition(3, 6);
	}

	//下と左上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum != topLeftNum && centerNum == topRightNum) {
			block->SetUVPosition(6, 6);
	}

	//下と右上以外ブロックあり(角は問わない)
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum &&
		centerNum == topLeftNum && centerNum != topRightNum) {
			block->SetUVPosition(7, 6);
	}

	//周囲四マスにブロックがある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(0, 3);
	}

	//周囲四マスに同一ブロックが無い場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(0, 0);
	}
	//右に1つだけある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(0, 2);
	}
	//左に1つだけある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(2, 2);
	}
	//上に1つだけある場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(3, 2);
	}
	//下に一つだけある場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(1, 2);
	}
	//右と下にある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(4, 5);
	}
	//左と下にある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(5, 5);
	}
	//右と上にある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(7, 5);
	}
	//左と上にある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(6, 5);
	}
	//左右にある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum != bottomNum) {
		block->SetUVPosition(5, 1);
	}
	//上下にある場合
	else if (centerNum != leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(4, 1);
	}
	//右以外にある場合
	else if (centerNum == leftNum && centerNum != rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(2, 5);
	}
	//左以外にある場合
	else if (centerNum != leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum == bottomNum) {
		block->SetUVPosition(0, 5);
	}
	//上以外にある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum != topNum && centerNum == bottomNum) {
		block->SetUVPosition(1, 5);
	}
	//下以外にある場合
	else if (centerNum == leftNum && centerNum == rightNum &&
		centerNum == topNum && centerNum != bottomNum) {
		block->SetUVPosition(3, 5);
	}

}
