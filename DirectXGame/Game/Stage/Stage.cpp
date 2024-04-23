#include "Stage.h"
#include <filesystem>
#include <iostream>
#include <string>
#include <fstream>
#include <cassert>
#include "Game/Player/Player.h"
#include "TextureManager.h"
#include <cmath>

std::array<std::array<std::shared_ptr<Block>, Stage::kMaxStageWidth_>, Stage::kMaxStageHeight_> Stage::map_;

Stage::Stage()
{

	numTex_ = TextureManager::GetInstance()->Load("UI/number.png");
	clearTex_ = TextureManager::GetInstance()->Load("UI/gameClear.png");
	borderTex_ = TextureManager::GetInstance()->Load("stageObject/line.png");
	magmaTex_ = TextureManager::GetInstance()->Load("stageObject/magmaLine.png");

	for (int32_t i = 0; i < 2; i++) {

		numbers_[i].reset(Sprite::Create(numTex_, {1150.0f + 64.0f * i, 660.0f}));
		numbers_[i]->SetSize({ 64.0f,64.0f });
		numbers_[i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

	}

	clearSprite_.reset(Sprite::Create(clearTex_, { 640.0f,360.0f }));

	borders_[0].reset(Object2d::Create(borderTex_, kBorderLeft));
	borders_[0]->SetAnchorpoint({ 0.5f,1.0f });
	borders_[1].reset(Object2d::Create(borderTex_, kBorderRight));
	borders_[1]->SetAnchorpoint({ 0.5f,1.0f });

	magma_.reset(Object2d::Create(magmaTex_, { kBasePosition.x,magmaLine_ }));
	magma_->SetColor({ 1.0f,1.0f,1.0f,0.8f });
	magma_->SetSize({ float(Block::kBlockSize_ * kMaxStageWidth_), 64.0f });
	magma_->SetTextureArea({ 0.0f,0.0f }, { float(Block::kBlockSize_ * kMaxStageWidth_), 32.0f });

}

Stage::~Stage()
{
}

void Stage::Initialize() {

	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			blockPositions_[y][x] = 0;

		}

	}

	CreateEntity();

	isClear_ = false;

}

void Stage::Update() {

	//パーツがなくなったらクリアフラグをセット
	if (remainingParts_ <= 0 && !player_->GetIsClear()) {
		player_->SetIsClear(true);
		isClear_ = true;
	}

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

	//拠点に帰った時にパーツを回収
	if (player_->GetIsHome() && player_->GetPartsCount() > 0) {
		player_->HandOverParts(remainingParts_);
	}

	//ブロックの更新
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {
			blockPositions_[y][x] = map_[y][x]->GetType();
			SetUV(map_[y][x].get());
			map_[y][x]->Update();
		}

	}

	//数字更新
	for (int32_t i = 0; i < kMaxNumber_; i++) {

		int32_t num = 0;

		int32_t divide = int32_t(std::pow(10, kMaxNumber_ - 1 - i));
		
		num = remainingParts_ / divide;

		numbers_[i]->SetTextureArea({64.0f * num, 0.0f}, { 64.0f,64.0f });

	}

	//マグマ更新
	magma_->SetSize({ float(Block::kBlockSize_ * kMaxStageWidth_), 10000.0f - magmaLine_ });
	magma_->SetTextureArea({ magmaTexBaseX_,0.0f }, { float(Block::kBlockSize_ * kMaxStageWidth_), 10000.0f - magmaLine_ });

	magmaLine_ -= 0.1f;

	magmaTexBaseX_++;
	if (magmaTexBaseX_ > 256.0f) {
		magmaTexBaseX_ = 0.0f;
	}

}

void Stage::Draw() {

	//ブロックの描画
	for (uint32_t y = 0; y < kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < kMaxStageWidth_; x++) {

			if (map_[y][x]->GetPosition().x >= camera_->translation_.x - Block::kBlockSize_ &&
				map_[y][x]->GetPosition().x <= camera_->translation_.x + camera_->GetDrawingRange().x + Block::kBlockSize_ &&
				map_[y][x]->GetPosition().y >= camera_->translation_.y - Block::kBlockSize_ &&
				map_[y][x]->GetPosition().y <= camera_->translation_.y + camera_->GetDrawingRange().y + Block::kBlockSize_) {
				map_[y][x]->Draw(*camera_);
			}
			
		}

	}

	for (uint32_t i = 0; i < 2; i++) {
		borders_[i]->Draw(*camera_);
	}

	magma_->Draw(*camera_);

}

void Stage::DrawUI() {

	for (uint32_t i = 0; i < 2; i++) {
		numbers_[i]->Draw();
	}

	if (isClear_) {
		clearSprite_->Draw();
	}

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

	//パーツの残り数をリセット
	remainingParts_ = 0;

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
			map_[y][x]->Reset();

			//ブロックがパーツなら残りのパーツ数を増加
			if (type == Block::BlockType::kParts) {
				remainingParts_++;
			}

			blockPositions_[y][x] = num;

		}

	}

}

void Stage::SetUV(Block* block) {

	int32_t px = block->GetBlockPositionX();
	int32_t py = block->GetBlockPositionY();

	//周囲八マスの判定を取るための変数
	int32_t left = px - 1;
	int32_t right = px + 1;
	int32_t top = py - 1;
	int32_t bottom = py + 1;
	
	//周囲八マスと現在のブロックの数字
	int32_t centerNum = blockPositions_[py][px];
	int32_t leftNum = 0;
	int32_t rightNum = 0;
	int32_t topNum = 0;
	int32_t bottomNum = 0;
	int32_t topLeftNum = 0;
	int32_t topRightNum = 0;
	int32_t bottomLeftNum = 0;
	int32_t bottomRightNum = 0;

	//範囲外に出るかどうかで代入する値を変更
	if (left < 0 || left >= kMaxStageWidth_) {

		leftNum = 0;
		topLeftNum = 0;
		bottomLeftNum = 0;


	}
	else {

		leftNum = blockPositions_[py][left];

	}

	if (right < 0 || right >= kMaxStageWidth_) {

		rightNum = 0;
		topRightNum = 0;
		bottomRightNum = 0;

	}
	else {

		rightNum = blockPositions_[py][right];

	}

	if (top < 0 || top >= kMaxStageHeight_) {

		topNum = 0;
		topLeftNum = 0;
		topRightNum = 0;

	}
	else {

		topNum = blockPositions_[top][px];

		if (left >= 0 && left < kMaxStageWidth_) {
			topLeftNum = blockPositions_[top][left];
		}

		if (right >= 0 && right < kMaxStageWidth_) {
			topRightNum = blockPositions_[top][right];
		}

	}

	if (bottom < 0 || bottom >= kMaxStageHeight_) {

		bottomNum = 0;
		bottomLeftNum = 0;
		bottomRightNum = 0;

	}
	else {

		bottomNum = blockPositions_[bottom][px];

		if (left >= 0 && left < kMaxStageWidth_) {
			bottomLeftNum = blockPositions_[bottom][left];
		}

		if (right >= 0 && right < kMaxStageWidth_) {
			bottomRightNum = blockPositions_[bottom][right];
		}

	}

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