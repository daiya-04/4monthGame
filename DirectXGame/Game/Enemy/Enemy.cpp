#include "Enemy.h"

BaseEnemy::BaseEnemy() {
	enemyTexture_ = TextureManager::GetInstance()->Load("enemy/enemy.png");
	object_.reset(Object2d::Create(enemyTexture_, {}));
}

void BaseEnemy::CheckCollision() {

	if (blocksPtr_) {

		int32_t startY = int32_t(tmpPosition_.y) / Block::kBlockSize_ - 2;
		startY = std::clamp(startY, 0, kMaxStageHeight - 1);
		int32_t endY = int32_t(tmpPosition_.y) / Block::kBlockSize_ + 2;
		endY = std::clamp(endY, 0, kMaxStageHeight - 1);
		int32_t startX = int32_t(tmpPosition_.x) / Block::kBlockSize_ - 2;
		startX = std::clamp(startX, 0, kMaxStageWidth - 1);
		int32_t endX = int32_t(tmpPosition_.x) / Block::kBlockSize_ + 2;
		endX = std::clamp(endX, 0, kMaxStageWidth - 1);

		for (int32_t y = startY; y <= endY; y++) {

			for (int32_t x = startX; x <= endX; x++) {

				//破壊されているものは処理しない
				if ((*blocksPtr_)[y][x]->GetIsBreak()) {
					continue;
				}

				//プレイヤーの周囲のみ当たり判定チェック
				if ((*blocksPtr_)[y][x]->GetPosition().x >= tmpPosition_.x - Block::kBlockSize_ * 3 &&
					(*blocksPtr_)[y][x]->GetPosition().x <= tmpPosition_.x + Block::kBlockSize_ * 3 &&
					(*blocksPtr_)[y][x]->GetPosition().y >= tmpPosition_.y - Block::kBlockSize_ * 3 &&
					(*blocksPtr_)[y][x]->GetPosition().y <= tmpPosition_.y + Block::kBlockSize_ * 3) {

					//当たり判定チェック
					if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), collision_)) {

						//崩れるブロックに触れたら崩壊開始
						/*if ((*blocksPtr_)[y][x]->GetType() == Block::kCollapseBlock && !(*blocksPtr_)[y][x]->GetIsStartCollapse()) {
							(*blocksPtr_)[y][x]->SetCollapse();
						}*/

						//左上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftTop_)) {

							//右側から侵入したなら右に押し戻し
							if (preLeftTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kEnemyHalfSize_), tmpPosition_.y });
								isMoveLeft_ = false;

								//ブロック破壊
								if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType()) && (*blocksPtr_)[y][x]->GetType() != Block::kGoldBlock) {
									(*blocksPtr_)[y][x]->Break(999, false);
								}

							}
							else {

								if (preLeftTop_.x > (*blocksPtr_)[y][x]->GetPosition().x + Block::kBlockHalfSize_ - 1) {



								}
								else {

									SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kEnemyHalfSize_) });
									velocity_.y = 0.0f;

								}

							}

						}
						//右上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightTop_)) {

							//左側から侵入したなら左に押し戻し
							if (preRightTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kEnemyHalfSize_), tmpPosition_.y });
								isMoveLeft_ = true;

								//ブロック破壊
								if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType()) && (*blocksPtr_)[y][x]->GetType() != Block::kGoldBlock) {
									(*blocksPtr_)[y][x]->Break(999, false);
								}

							}
							else {

								if (preRightTop_.x < (*blocksPtr_)[y][x]->GetPosition().x - Block::kBlockHalfSize_) {



								}
								else {

									SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kEnemyHalfSize_) });
									velocity_.y = 0.0f;

								}

							}

						}

						//左下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftBottom_)) {

							//右側から侵入したなら右に押し戻し
							if (preLeftBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kEnemyHalfSize_), tmpPosition_.y });
								isMoveLeft_ = false;

								//ブロック破壊
								if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType()) && (*blocksPtr_)[y][x]->GetType() != Block::kGoldBlock) {
									(*blocksPtr_)[y][x]->Break(999, false);
								}

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kEnemyHalfSize_) });
								velocity_.y = 0.0f;

							}

						}

						//右下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightBottom_)) {

							//左側から侵入したなら左に押し戻し
							if (preRightBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kEnemyHalfSize_), tmpPosition_.y });
								isMoveLeft_ = true;

								//ブロック破壊
								if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType()) && (*blocksPtr_)[y][x]->GetType() != Block::kGoldBlock) {
									(*blocksPtr_)[y][x]->Break(999, false);
								}

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kEnemyHalfSize_) });
								velocity_.y = 0.0f;

							}

						}

					}
					
				}

			}

		}

	}

}

void NormalEnemy::Initialize(const Vector2& position) {

	position_ = position;
	object_->position_ = position_;
	
	tmpPosition_ = position_;
	prePosition_ = position_;

	collision_.min = { position_.x - kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
	collision_.max = { position_.x + kEnemyHalfSize_, position_.y + kEnemyHalfSize_ };

	leftTop_ = { position_.x - kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
	rightTop_ = { position_.x + kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
	leftBottom_ = { position_.x - kEnemyHalfSize_, position_.y + kEnemyHalfSize_ };
	rightBottom_ = { position_.x + kEnemyHalfSize_, position_.y + kEnemyHalfSize_ };

	preLeftTop_ = { prePosition_.x - kEnemyHalfSize_, prePosition_.y - kEnemyHalfSize_ };
	preRightTop_ = { prePosition_.x + kEnemyHalfSize_, prePosition_.y - kEnemyHalfSize_ };
	preLeftBottom_ = { prePosition_.x - kEnemyHalfSize_, prePosition_.y + kEnemyHalfSize_ };
	preRightBottom_ = { prePosition_.x + kEnemyHalfSize_, prePosition_.y + kEnemyHalfSize_ };

	velocity_ = { 0.0f,0.0f };

	isMoveLeft_ = false;


}

void NormalEnemy::Update() {

	//前フレームの位置をセット
	prePosition_ = position_;

	preLeftTop_ = leftTop_;
	preRightTop_ = rightTop_;
	preLeftBottom_ = leftBottom_;
	preRightBottom_ = rightBottom_;

	if (isMoveLeft_) {
		velocity_.x = -1.0f;
	}
	else {
		velocity_.x = 1.0f;
	}

	if (velocity_.y < maxFallVelocity_) {

		velocity_.y += gravityFallSpeed_;

		//下限値を超えないように調整
		if (velocity_.y > maxFallVelocity_) {
			velocity_.y = maxFallVelocity_;
		}

	}

	tmpPosition_ = position_;

	tmpPosition_ += velocity_;

	//4頂点の座標を更新
	leftTop_ = { tmpPosition_.x - kEnemyHalfSize_, tmpPosition_.y - kEnemyHalfSize_ };
	rightTop_ = { tmpPosition_.x + kEnemyHalfSize_ - 1, tmpPosition_.y - kEnemyHalfSize_ };
	leftBottom_ = { tmpPosition_.x - kEnemyHalfSize_, tmpPosition_.y + kEnemyHalfSize_ - 1 };
	rightBottom_ = { tmpPosition_.x + kEnemyHalfSize_ - 1, tmpPosition_.y + kEnemyHalfSize_ - 1 };

	//仮の当たり判定更新
	collision_.min = { tmpPosition_.x - kEnemyHalfSize_, tmpPosition_.y - kEnemyHalfSize_ };
	collision_.max = { tmpPosition_.x + kEnemyHalfSize_ - 1, tmpPosition_.y + kEnemyHalfSize_ - 1 };

	CheckCollision();

	position_ = tmpPosition_;

	object_->position_ = position_;

	//4頂点の座標を更新
	leftTop_ = { position_.x - kEnemyHalfSize_, position_.y - kEnemyHalfSize_ };
	rightTop_ = { position_.x + kEnemyHalfSize_ - 1, position_.y - kEnemyHalfSize_ };
	leftBottom_ = { position_.x - kEnemyHalfSize_, position_.y + kEnemyHalfSize_ - 1 };
	rightBottom_ = { position_.x + kEnemyHalfSize_ - 1, position_.y + kEnemyHalfSize_ - 1 };

}

void NormalEnemy::Draw(const Camera& camera) {

	object_->Draw(camera);

}
