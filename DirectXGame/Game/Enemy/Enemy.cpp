#include "Enemy.h"
#include "Object2dInstancing.h"
BaseEnemy::BaseEnemy() {
	enemyTexture_ = TextureManager::GetInstance()->Load("enemy/enemy.png");
	crushSE_ = AudioManager::GetInstance()->Load("SE/crush.mp3");
}

void BaseEnemy::CheckCollision() {

	//ブロックとの当たり判定
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

				//周囲のみ当たり判定チェック
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

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_), tmpPosition_.y });
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

									SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_) });
									velocity_.y = 0.0f;

								}

							}

						}
						//右上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightTop_)) {

							//左側から侵入したなら左に押し戻し
							if (preRightTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_), tmpPosition_.y });
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

									SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_) });
									velocity_.y = 0.0f;

								}

							}

						}

						//左下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftBottom_)) {

							//右側から侵入したなら右に押し戻し
							if (preLeftBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_), tmpPosition_.y });
								isMoveLeft_ = false;

								//ブロック破壊
								if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType()) && (*blocksPtr_)[y][x]->GetType() != Block::kGoldBlock) {
									(*blocksPtr_)[y][x]->Break(999, false);
								}

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_) });
								velocity_.y = 0.0f;

							}

						}

						//右下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightBottom_)) {

							//左側から侵入したなら左に押し戻し
							if (preRightBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_), tmpPosition_.y });
								isMoveLeft_ = true;

								//ブロック破壊
								if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType()) && (*blocksPtr_)[y][x]->GetType() != Block::kGoldBlock) {
									(*blocksPtr_)[y][x]->Break(999, false);
								}

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kEnemyMiddleHalfSize_) });
								velocity_.y = 0.0f;

							}

						}

					}
					
				}

			}

		}

	}

	//プレイヤーとの当たり判定
	if (player_) {

		//プレイヤーとの距離が一定まで近づいていたら判定を取る
		if (fabsf(player_->GetPosition().x - position_.x) <= 150.0f &&
			fabsf(player_->GetPosition().y - position_.y) <= 150.0f) {

			//プレイヤーと当たっていたら
			if (IsCollision(player_->GetCollision(), collision_)) {

				//上側からプレイヤーが来ていたら踏み判定
				if (player_->GetPrePosition().y + Player::kPlayerHalfSizeY_ <= position_.y - kEnemyMiddleHalfSize_) {

					isDead_ = true;
					crushSE_->Play();
					player_->SetVelocityY(-20.0f);

				}
				else {

					//プレイヤースタン
					player_->Stun();

				}

			}

		}

	}

}

void BaseEnemy::SetDrawData(Object2dInstancing* func) {
	Vector2 size;
	size.x = rightTop_.x - leftTop_.x;
	size.y = rightBottom_.y - rightTop_.y;
	Vector2 texBase = {0,0};
	Vector2 texSize = texSize_;
	if (isMoveLeft_) {
		texBase.x = texSize.x;
		texSize.x =-1.0f * texSize.x;
	}
	func->AppendObject(position_,size, 0, texBase, texSize, {1.0f,1.0f,1.0f,1.0f}, 0);
}

void NormalEnemy::Initialize(const Vector2& position) {

	position_ = position;
	
	tmpPosition_ = position_;
	prePosition_ = position_;

	collision_.min = { position_.x - kEnemyMiddleHalfSize_, position_.y - kEnemyMiddleHalfSize_ };
	collision_.max = { position_.x + kEnemyMiddleHalfSize_, position_.y + kEnemyMiddleHalfSize_ };

	leftTop_ = { position_.x - kEnemyMiddleHalfSize_, position_.y - kEnemyMiddleHalfSize_ };
	rightTop_ = { position_.x + kEnemyMiddleHalfSize_, position_.y - kEnemyMiddleHalfSize_ };
	leftBottom_ = { position_.x - kEnemyMiddleHalfSize_, position_.y + kEnemyMiddleHalfSize_ };
	rightBottom_ = { position_.x + kEnemyMiddleHalfSize_, position_.y + kEnemyMiddleHalfSize_ };

	preLeftTop_ = { prePosition_.x - kEnemyMiddleHalfSize_, prePosition_.y - kEnemyMiddleHalfSize_ };
	preRightTop_ = { prePosition_.x + kEnemyMiddleHalfSize_, prePosition_.y - kEnemyMiddleHalfSize_ };
	preLeftBottom_ = { prePosition_.x - kEnemyMiddleHalfSize_, prePosition_.y + kEnemyMiddleHalfSize_ };
	preRightBottom_ = { prePosition_.x + kEnemyMiddleHalfSize_, prePosition_.y + kEnemyMiddleHalfSize_ };

	velocity_ = { 0.0f,0.0f };

	isMoveLeft_ = false;

	texSize_ = {64.0f,64.0f};
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
	leftTop_ = { tmpPosition_.x - kEnemyMiddleHalfSize_, tmpPosition_.y - kEnemyMiddleHalfSize_ };
	rightTop_ = { tmpPosition_.x + kEnemyMiddleHalfSize_ - 1, tmpPosition_.y - kEnemyMiddleHalfSize_ };
	leftBottom_ = { tmpPosition_.x - kEnemyMiddleHalfSize_, tmpPosition_.y + kEnemyMiddleHalfSize_ - 1 };
	rightBottom_ = { tmpPosition_.x + kEnemyMiddleHalfSize_ - 1, tmpPosition_.y + kEnemyMiddleHalfSize_ - 1 };

	//仮の当たり判定更新
	collision_.min = { tmpPosition_.x - kEnemyMiddleHalfSize_, tmpPosition_.y - kEnemyMiddleHalfSize_ };
	collision_.max = { tmpPosition_.x + kEnemyMiddleHalfSize_ - 1, tmpPosition_.y + kEnemyMiddleHalfSize_ - 1 };

	CheckCollision();

	position_ = tmpPosition_;

	
	//4頂点の座標を更新
	leftTop_ = { position_.x - kEnemyMiddleHalfSize_, position_.y - kEnemyMiddleHalfSize_ };
	rightTop_ = { position_.x + kEnemyMiddleHalfSize_ - 1, position_.y - kEnemyMiddleHalfSize_ };
	leftBottom_ = { position_.x - kEnemyMiddleHalfSize_, position_.y + kEnemyMiddleHalfSize_ - 1 };
	rightBottom_ = { position_.x + kEnemyMiddleHalfSize_ - 1, position_.y + kEnemyMiddleHalfSize_ - 1 };

}

void NormalEnemy::Draw(const Camera& camera) {

}

void NeedleEnemy::Initialize(const Vector2& position) {

	position_ = position;
	
	tmpPosition_ = position_;
	prePosition_ = position_;

	collision_.min = { position_.x - kEnemyLargeHalfSize_, position_.y - kEnemyLargeHalfSize_ };
	collision_.max = { position_.x + kEnemyLargeHalfSize_, position_.y + kEnemyLargeHalfSize_ };

	leftTop_ = { position_.x - kEnemyLargeHalfSize_, position_.y - kEnemyLargeHalfSize_ };
	rightTop_ = { position_.x + kEnemyLargeHalfSize_, position_.y - kEnemyLargeHalfSize_ };
	leftBottom_ = { position_.x - kEnemyLargeHalfSize_, position_.y + kEnemyLargeHalfSize_ };
	rightBottom_ = { position_.x + kEnemyLargeHalfSize_, position_.y + kEnemyLargeHalfSize_ };

	preLeftTop_ = { prePosition_.x - kEnemyLargeHalfSize_, prePosition_.y - kEnemyLargeHalfSize_ };
	preRightTop_ = { prePosition_.x + kEnemyLargeHalfSize_, prePosition_.y - kEnemyLargeHalfSize_ };
	preLeftBottom_ = { prePosition_.x - kEnemyLargeHalfSize_, prePosition_.y + kEnemyLargeHalfSize_ };
	preRightBottom_ = { prePosition_.x + kEnemyLargeHalfSize_, prePosition_.y + kEnemyLargeHalfSize_ };

	velocity_ = { 0.0f,0.0f };

	isMoveLeft_ = false;

	enemyTexture_ = TextureManager::GetInstance()->Load("blocks/needle.png");
	type_ = Type::kNeedle;

	texSize_ = { 32.0f,32.0f };
}

void NeedleEnemy::Update() {

	//前フレームの位置をセット
	prePosition_ = position_;

	preLeftTop_ = leftTop_;
	preRightTop_ = rightTop_;
	preLeftBottom_ = leftBottom_;
	preRightBottom_ = rightBottom_;

	if (isStartFall_) {
		
		if (startFallCount_ > 0) {
			startFallCount_--;
		}
		else {

			if (velocity_.y < maxFallVelocity_) {

				velocity_.y += gravityFallSpeed_;

				//下限値を超えないように調整
				if (velocity_.y > maxFallVelocity_) {
					velocity_.y = maxFallVelocity_;
				}

			}

		}

	}

	tmpPosition_ = position_;

	tmpPosition_ += velocity_;

	//4頂点の座標を更新
	leftTop_ = { tmpPosition_.x - kEnemyLargeHalfSize_, tmpPosition_.y - kEnemyLargeHalfSize_ };
	rightTop_ = { tmpPosition_.x + kEnemyLargeHalfSize_ - 1, tmpPosition_.y - kEnemyLargeHalfSize_ };
	leftBottom_ = { tmpPosition_.x - kEnemyLargeHalfSize_, tmpPosition_.y + kEnemyLargeHalfSize_ - 1 };
	rightBottom_ = { tmpPosition_.x + kEnemyLargeHalfSize_ - 1, tmpPosition_.y + kEnemyLargeHalfSize_ - 1 };

	//仮の当たり判定更新
	collision_.min = { tmpPosition_.x - kEnemyLargeHalfSize_, tmpPosition_.y - kEnemyLargeHalfSize_ };
	collision_.max = { tmpPosition_.x + kEnemyLargeHalfSize_ - 1, tmpPosition_.y + kEnemyLargeHalfSize_ - 1 };

	CheckCollision();

	position_ = tmpPosition_;

	
	//4頂点の座標を更新
	leftTop_ = { position_.x - kEnemyLargeHalfSize_, position_.y - kEnemyLargeHalfSize_ };
	rightTop_ = { position_.x + kEnemyLargeHalfSize_ - 1, position_.y - kEnemyLargeHalfSize_ };
	leftBottom_ = { position_.x - kEnemyLargeHalfSize_, position_.y + kEnemyLargeHalfSize_ - 1 };
	rightBottom_ = { position_.x + kEnemyLargeHalfSize_ - 1, position_.y + kEnemyLargeHalfSize_ - 1 };
	//当たり判定
	AABB2D collision = collision_;
	collision.min.x -= 1.0f;
	collision.min.y -= 1.0f;
	collision.max.x += 1.0f;
	collision.max.y += 1.0f;
	if (player_->GetPrePosition() != player_->GetPosition() && IsCollision(collision, player_->GetCollision())) {
		Vector2 velocity = player_->GetPosition() - position_;
		Vector2 pos = position_ + velocity * 0.7f;
		velocity = velocity.Normalize();
		BlockTextureManager::GetInstance()->CreateSandParticle(pos, 0);
	}
}

void NeedleEnemy::Draw(const Camera& camera) {

}

void NeedleEnemy::CheckCollision() {

	if (keeper_) {

		//キーパーが破壊されていない場合、必ず止まっているのでreturnする
		if (!keeper_->GetIsBreak()) {
			isStartFall_ = false;
			return;
		}

	}

	isStartFall_ = true;

	//ブロックとの当たり判定
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

				//周囲のみ当たり判定チェック
				if ((*blocksPtr_)[y][x]->GetPosition().x >= tmpPosition_.x - Block::kBlockSize_ * 3 &&
					(*blocksPtr_)[y][x]->GetPosition().x <= tmpPosition_.x + Block::kBlockSize_ * 3 &&
					(*blocksPtr_)[y][x]->GetPosition().y >= tmpPosition_.y - Block::kBlockSize_ * 3 &&
					(*blocksPtr_)[y][x]->GetPosition().y <= tmpPosition_.y + Block::kBlockSize_ * 3) {

					//当たり判定チェック
					if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), collision_)) {

						keeper_ = (*blocksPtr_)[y][x].get();

						//崩れるブロックに触れたら崩壊開始
						/*if ((*blocksPtr_)[y][x]->GetType() == Block::kCollapseBlock && !(*blocksPtr_)[y][x]->GetIsStartCollapse()) {
							(*blocksPtr_)[y][x]->SetCollapse();
						}*/

						//左上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftTop_)) {

							//右側から侵入したなら右に押し戻し
							if (preLeftTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_), tmpPosition_.y });
								isMoveLeft_ = false;

							}
							else {

								if (preLeftTop_.x > (*blocksPtr_)[y][x]->GetPosition().x + Block::kBlockHalfSize_ - 1) {



								}
								else {

									SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_) });
									velocity_.y = 0.0f;

								}

							}

						}
						//右上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightTop_)) {

							//左側から侵入したなら左に押し戻し
							if (preRightTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_), tmpPosition_.y });
								isMoveLeft_ = true;

							}
							else {

								if (preRightTop_.x < (*blocksPtr_)[y][x]->GetPosition().x - Block::kBlockHalfSize_) {



								}
								else {

									SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_) });
									velocity_.y = 0.0f;

								}

							}

						}

						//左下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftBottom_)) {

							//右側から侵入したなら右に押し戻し
							if (preLeftBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_), tmpPosition_.y });
								isMoveLeft_ = false;

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_) });
								velocity_.y = 0.0f;
								isStartFall_ = false;
								startFallCount_ = 60;

							}

						}

						//右下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightBottom_)) {

							//左側から侵入したなら左に押し戻し
							if (preRightBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_), tmpPosition_.y });
								isMoveLeft_ = true;

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kEnemyLargeHalfSize_) });
								velocity_.y = 0.0f;
								isStartFall_ = false;
								startFallCount_ = 60;

							}

						}

					}

				}

			}

		}

	}

	//プレイヤーとの当たり判定
	if (player_) {

		//プレイヤーとの距離が一定まで近づいていたら判定を取る
		if (fabsf(player_->GetPosition().x - position_.x) <= 150.0f &&
			fabsf(player_->GetPosition().y - position_.y) <= 150.0f) {

			//プレイヤーと当たっていたら
			if (IsCollision(player_->GetCollision(), collision_)) {

				//下側からプレイヤーが来ていたらとげに当たったとみなし、スタートに戻す
				if (player_->GetPrePosition().y - Player::kPlayerHalfSizeY_ >= position_.y + kEnemyLargeHalfSize_) {

					player_->Restart();

				}
				else {

					

				}

			}

		}

	}

}
