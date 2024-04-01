#include "Player.h"
#include "TextureManager.h"
#include "imgui.h"
#include "Game/Block/Block.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize() {

	input_ = Input::GetInstance();

	texture_ = TextureManager::GetInstance()->Load("player/player.png");

	position_ = { 200.0f,200.0f };
	tmpPosition_ = position_;
	prePosition_ = position_;

	collision_.min = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	collision_.max = { position_.x + kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };

	object_.reset(Object2d::Create(texture_, position_));
	object_->SetSize({ kPlayerSize_, kPlayerSize_ });

	leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	rightTop_ = { position_.x + kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };
	rightBottom_ = { position_.x + kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };

	preLeftTop_ = { prePosition_.x - kPlayerHalfSize_, prePosition_.y - kPlayerHalfSize_ };
	preRightTop_ = { prePosition_.x + kPlayerHalfSize_, prePosition_.y - kPlayerHalfSize_ };
	preLeftBottom_ = { prePosition_.x - kPlayerHalfSize_, prePosition_.y + kPlayerHalfSize_ };
	preRightBottom_ = { prePosition_.x + kPlayerHalfSize_, prePosition_.y + kPlayerHalfSize_ };

	velocity_ = { 0.0f,0.0f };

}

void Player::Update(uint32_t(&map)[Stage::kMaxStageHeight_][Stage::kMaxStageWidth_]) {

	prePosition_ = position_;

	preLeftTop_ = leftTop_;
	preRightTop_ = rightTop_;
	preLeftBottom_ = leftBottom_;
	preRightBottom_ = rightBottom_;

	if (isDebug_) {

		useCollision_ = false;

		if (input_->PushKey(DIK_W)) {
			position_.y -= 10.0f;
		}
		if (input_->PushKey(DIK_S)) {
			position_.y += 10.0f;
		}
		if (input_->PushKey(DIK_A)) {
			position_.x -= 10.0f;
		}
		if (input_->PushKey(DIK_D)) {
			position_.x += 10.0f;
		}

	}
	else {

		useCollision_ = true;

		//ジャンプ処理
		Jump();

		//壁キック処理
		WallJump();

		//移動処理
		Move();

		//速度加算、オブジェクトに更新した座標を適用
		{

			//落下処理
			if (velocity_.y < maxFallSpeed_) {

				velocity_.y += kGravityFallSpeed_;

				//下限値を超えないように調整
				if (velocity_.y > maxFallSpeed_) {
					velocity_.y = maxFallSpeed_;
				}



			}

			//浮いていない場合の処理
			if (!isFly_) {

			}

			tmpPosition_ = position_;

			tmpPosition_ += velocity_;

			//4頂点の座標を更新
			leftTop_ = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y - kPlayerHalfSize_ };
			rightTop_ = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y - kPlayerHalfSize_ };
			leftBottom_ = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y + kPlayerHalfSize_ - 1 };
			rightBottom_ = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y + kPlayerHalfSize_ - 1 };

			//仮の当たり判定更新
			collision_.min = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y - kPlayerHalfSize_ };
			collision_.max = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y + kPlayerHalfSize_ - 1 };

			CheckCollision(map);

			position_ = tmpPosition_;

			object_->position_ = position_;

			//4頂点の座標を更新
			leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
			rightTop_ = { position_.x + kPlayerHalfSize_ - 1, position_.y - kPlayerHalfSize_ };
			leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ - 1 };
			rightBottom_ = { position_.x + kPlayerHalfSize_ - 1, position_.y + kPlayerHalfSize_ - 1 };

		}

		//判定処理
		{

			/*if (leftBottom_.y >= 720.0f) {
				position_.y = 720.0f - kPlayerHalfSize_;
				parameter_.Jump_.canJump = true;
				velocity_.y = 0.0f;
				isFly_ = false;
			}
			else {
				isFly_ = true;
			}*/

		}
		
	}

	collision_.min = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	collision_.max = { position_.x + kPlayerHalfSize_ - 1, position_.y + kPlayerHalfSize_ - 1 };

}

void Player::Draw(const Camera& camera) {

	object_->Draw(camera);

}

void Player::Move() {

	//移動
	if (input_->TiltLStick(Input::Stick::Right)) {
		velocity_.x = parameter_.speed_;
	}
	else if (input_->TiltLStick(Input::Stick::Left)) {
		velocity_.x = -parameter_.speed_;
	}
	else {
		velocity_.x = 0.0f;
	}

}

void Player::Jump() {

	//落下速度が0.1f以上あったら空中判定とみなし、ジャンプできなくする
	if (velocity_.y > 0.1f) {
		parameter_.Jump_.canJump = false;
	}

	//通常ジャンプ
	if (parameter_.Jump_.canJump && input_->TriggerButton(Input::Button::A)) {
		velocity_.y += parameter_.Jump_.jumpVelocity;
		parameter_.Jump_.canJump = false;
		isFly_ = true;
	}

}

void Player::WallJump() {

	//壁キック
	if (parameter_.wallJump_.canWallJump && input_->TriggerButton(Input::Button::A)) {

	}

}

void Player::CheckCollision(uint32_t(&map)[Stage::kMaxStageHeight_][Stage::kMaxStageWidth_]) {

	Vector2 tmp = position_;

	UpdateGrid();

	/*for (uint32_t y = 0; y < Stage::kMaxStageHeight_; y++) {

		for (uint32_t x = 0; x < Stage::kMaxStageWidth_; x++) {

			if (map[topGrid_][leftGrid_] != 0) {

			}



		}

	}*/

	if (blocksPtr_) {

		for (auto& block : *blocksPtr_) {

			//プレイヤーの周囲のみ当たり判定チェック
			if (block->GetPosition().x >= tmpPosition_.x - Block::kBlockSize_ * 3 &&
				block->GetPosition().x <= tmpPosition_.x + Block::kBlockSize_ * 3 &&
				block->GetPosition().y >= tmpPosition_.y - Block::kBlockSize_ * 3 &&
				block->GetPosition().y <= tmpPosition_.y + Block::kBlockSize_ * 3) {

				//当たり判定チェック
				if (IsCollision(block->GetCollision(), collision_)) {

					if (block->isDebug_) {

						block->SetColor({ 1.0f,0.0f,0.0f,1.0f });

					}

					//左上が当たっていた
					if (IsCollision(block->GetCollision(), leftTop_)) {

						//プレイヤーが左側から侵入したなら右に押し戻し
						if (preLeftTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

							velocity_.x = 0;

							SetTmpPosition({ block->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

						}
						else {

							velocity_.y = 0;

							SetTmpPosition({ tmpPosition_.x,block->GetPosition().y + (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

						}

						/*if (leftTop_.x < block->GetPosition().x + Block::kBlockHalfSize_ - 1) {
							
							velocity_.x = 0;

							SetTmpPosition({ block->GetPosition().x + Block::kBlockSize_, tmpPosition_.y });

						}

						プレイヤーがブロックより上側にいたなら押し戻し
						if (leftTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {
							
							velocity_.y = 0;

							SetTmpPosition({ tmpPosition_.x,block->GetPosition().y + Block::kBlockSize_ });

						}*/

					}
					//右上が当たっていた
					if (IsCollision(block->GetCollision(), rightTop_)) {

						//プレイヤーが右側から侵入したなら左に押し戻し
						if (preRightTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

							velocity_.x = 0;

							SetTmpPosition({ block->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

						}
						else {

							velocity_.y = 0;

							SetTmpPosition({ tmpPosition_.x,block->GetPosition().y + (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

						}

						////プレイヤーがブロックより右側にいたなら押し戻し
						//if (rightTop_.x > block->GetPosition().x - Block::kBlockHalfSize_) {

						//	velocity_.x = 0;

						//	SetTmpPosition({ block->GetPosition().x - Block::kBlockSize_, tmpPosition_.y });

						//}

						////プレイヤーがブロックより上側にいたなら押し戻し
						//if (rightTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

						//	velocity_.y = 0;

						//	SetTmpPosition({ tmpPosition_.x,block->GetPosition().y + Block::kBlockSize_ });

						//}

					}

					//左下が当たっていた
					if (IsCollision(block->GetCollision(), leftBottom_)) {

						//プレイヤーが左側から侵入したなら右に押し戻し
						if (preLeftBottom_.y > block->GetPosition().y - Block::kBlockHalfSize_) {

							velocity_.x = 0;

							SetTmpPosition({ block->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

						}
						else {

							velocity_.y = 0;

							SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

							SetCanJump(true);
							SetIsFly(false);

						}

						////プレイヤーがブロックより左側にいたなら押し戻し
						//if (leftBottom_.x < block->GetPosition().x + Block::kBlockHalfSize_ - 1) {

						//	velocity_.x = 0;

						//	SetTmpPosition({ block->GetPosition().x + Block::kBlockSize_, tmpPosition_.y });

						//}

						////プレイヤーがブロックより下側にいたなら押し戻し
						//if (leftBottom_.y > block->GetPosition().y - Block::kBlockHalfSize_) {

						//	velocity_.y = 0;

						//	SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - Block::kBlockSize_ });

						//	SetCanJump(true);
						//	SetIsFly(false);

						//}

					}

					//右下が当たっていた
					if (IsCollision(block->GetCollision(), rightBottom_)) {

						//プレイヤーが右側から侵入したなら左に押し戻し
						if (preRightBottom_.y > block->GetPosition().y - Block::kBlockHalfSize_) {

							velocity_.x = 0;

							SetTmpPosition({ block->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

						}
						else {

							velocity_.y = 0;

							SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

							SetCanJump(true);
							SetIsFly(false);

						}

						////プレイヤーがブロックより右側にいたなら押し戻し
						//if (rightBottom_.x > block->GetPosition().x - Block::kBlockHalfSize_) {

						//	velocity_.x = 0;

						//	SetTmpPosition({ block->GetPosition().x - Block::kBlockSize_, tmpPosition_.y });

						//}

						////プレイヤーがブロックより下側にいたなら押し戻し
						//if (rightBottom_.y > block->GetPosition().y - Block::kBlockHalfSize_) {

						//	velocity_.y = 0;

						//	SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - Block::kBlockSize_ });

						//	SetCanJump(true);
						//	SetIsFly(false);

						//}

					}

					/*player_->SetPosition(tmp);*/

				}
				else {

					block->SetColor({ 1.0f,1.0f,1.0f,1.0f });

				}

			}
			else {

				block->SetColor({ 1.0f,1.0f,1.0f,1.0f });

			}

		}

	}

	/*position_ = tmp;*/

}

void Player::UpdateGrid() {

	leftGrid_ = int(leftTop_.x) / Block::kBlockSize_;
	rightGrid_ = int(rightBottom_.x - 1) / Block::kBlockSize_;
	topGrid_ = int(leftTop_.y) / Block::kBlockSize_;
	bottomGrid_ = int(rightBottom_.y - 1) / Block::kBlockSize_;

}

void Player::Debug() {

#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::Text("position x : %1.2f y : %1.2f", position_.x, position_.y);
	ImGui::End();

#endif // _DEBUG

}
