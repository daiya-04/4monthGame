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
	textureLeft_ = TextureManager::GetInstance()->Load("player/playerLeft.png");
	textureRight_ = TextureManager::GetInstance()->Load("player/playerRight.png");
	textureUp_ = TextureManager::GetInstance()->Load("player/playerUp.png");
	textureDown_ = TextureManager::GetInstance()->Load("player/playerDown.png");

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

void Player::Update() {

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

		//移動処理
		Move();

		//ジャンプ処理
		Jump();

		//壁キック処理
		WallJump();

		//穴掘り処理
		Dig();

		//速度加算、オブジェクトに更新した座標を適用
		{

			//左右速度の制限
			if (std::fabsf(velocity_.x) > kMaxMoveSpeed_) {

				if (velocity_.x > 0.0f) {
					velocity_.x = kMaxMoveSpeed_;
				}
				else {
					velocity_.x = -kMaxMoveSpeed_;
				}

			}

			//壁キックの左右速度調整
			if (std::fabsf(wallJumpVelocity_.x) > 1.0f) {

				if (wallJumpVelocity_.x > 0.0f) {
					wallJumpVelocity_.x -= 0.3f;
				}
				else {
					wallJumpVelocity_.x += 0.3f;
				}

			}
			else {
				wallJumpVelocity_.x = 0.0f;
			}

			//左右移動の合計速度が上限を超えないように制限
			if (std::fabsf(wallJumpVelocity_.x + velocity_.x) > kMaxMoveSpeed_) {

				if (wallJumpVelocity_.x + velocity_.x > 0.0f) {
					velocity_.x = kMaxMoveSpeed_ - wallJumpVelocity_.x;
				}
				else {
					velocity_.x = -kMaxMoveSpeed_ - wallJumpVelocity_.x;
				}

			}

			//落下処理
			if (velocity_.y < kMaxFallSpeed_) {

				velocity_.y += kGravityFallSpeed_;

				//下限値を超えないように調整
				if (velocity_.y > kMaxFallSpeed_) {
					velocity_.y = kMaxFallSpeed_;
				}

			}

			//溜めジャンプの破壊フラグが経っている状態で、速度が規定値を超えたらフラグを下す
			if (parameter_.chargeJump_.canBreak && velocity_.y > parameter_.chargeJump_.unBreakVelocity) {
				parameter_.chargeJump_.canBreak = false;
			}

			//壁キック可能時(壁ずりしている時)、下方向の落下速度を減少させる
			if (parameter_.wallJump_.canWallJump) {

				velocity_.y *= 0.6f;

			}

			//浮いていない場合の処理
			if (!isFly_) {

			}

			tmpPosition_ = position_;

			tmpPosition_ += velocity_ + wallJumpVelocity_;

			//4頂点の座標を更新
			leftTop_ = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y - kPlayerHalfSize_ };
			rightTop_ = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y - kPlayerHalfSize_ };
			leftBottom_ = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y + kPlayerHalfSize_ - 1 };
			rightBottom_ = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y + kPlayerHalfSize_ - 1 };

			//仮の当たり判定更新
			collision_.min = { tmpPosition_.x - kPlayerHalfSize_, tmpPosition_.y - kPlayerHalfSize_ };
			collision_.max = { tmpPosition_.x + kPlayerHalfSize_ - 1, tmpPosition_.y + kPlayerHalfSize_ - 1 };

			CheckCollision();

			position_ = tmpPosition_;

			object_->position_ = position_;

			//4頂点の座標を更新
			leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
			rightTop_ = { position_.x + kPlayerHalfSize_ - 1, position_.y - kPlayerHalfSize_ };
			leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ - 1 };
			rightBottom_ = { position_.x + kPlayerHalfSize_ - 1, position_.y + kPlayerHalfSize_ - 1 };

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
	if (input_->TiltLStick(Input::Stick::Right) && wallJumpVelocity_.x >= -1.0f) {

		if (velocity_.x < -2.0f) {
			velocity_.x = 0.0f;
		}

		velocity_.x += parameter_.speed_;

		isFacingLeft_ = false;

	}
	else if (input_->TiltLStick(Input::Stick::Left) && wallJumpVelocity_.x <= 1.0f) {

		if (velocity_.x > 2.0f) {
			velocity_.x = 0.0f;
		}

		velocity_.x -= parameter_.speed_;

		isFacingLeft_ = true;

	}
	else {

		//減速処理
		if (velocity_.x > 2.5f) {
			velocity_.x -= 2.0f;
		}
		else if (velocity_.x < -2.5f) {
			velocity_.x += 2.0f;
		}
		else {
			velocity_.x = 0.0f;
		}

	}

	if (parameter_.Jump_.canJump && input_->TiltLStick(Input::Stick::Up)) {

		parameter_.chargeJump_.isCharge = true;

	}
	else {
		parameter_.chargeJump_.isCharge = false;
	}

	//入力方向に応じて画像変更
	if (input_->TiltLStick(Input::Stick::Right)) {
		object_->SetTextureHandle(textureRight_);
	}
	else if (input_->TiltLStick(Input::Stick::Left)) {
		object_->SetTextureHandle(textureLeft_);
	}
	else if (input_->TiltLStick(Input::Stick::Up)) {
		object_->SetTextureHandle(textureUp_);
	}
	else if (input_->TiltLStick(Input::Stick::Down)) {
		object_->SetTextureHandle(textureDown_);
	}
	else {
		object_->SetTextureHandle(texture_);
	}

}

void Player::Jump() {

	//落下速度が0.1f以上あったら空中判定とみなし、ジャンプできなくする
	if (velocity_.y > 0.1f) {
		parameter_.Jump_.canJump = false;
	}

	//通常ジャンプ
	if (parameter_.Jump_.canJump && input_->TriggerButton(Input::Button::A) && !parameter_.chargeJump_.isCharge) {
		velocity_.y += parameter_.Jump_.jumpVelocity;
		parameter_.Jump_.canJump = false;
		isFly_ = true;
	}
	//溜めジャンプが可能なときに必要な溜めの時間まで行かなかったら通常ジャンプの処理に切り替え
	else if (parameter_.Jump_.canJump && input_->ReleaseButton(Input::Button::A) && parameter_.chargeJump_.isCharge &&
		parameter_.chargeJump_.chargeTimer < parameter_.chargeJump_.kMaxChargeTime) {
		velocity_.y += parameter_.Jump_.jumpVelocity;
		parameter_.Jump_.canJump = false;
		isFly_ = true;
	}

	//溜めジャンプ処理
	ChargeJump();

}

void Player::ChargeJump() {

	//チャージ完了した状態でボタンを離した時に溜めジャンプ発動
	if (parameter_.chargeJump_.chargeTimer == parameter_.chargeJump_.kMaxChargeTime &&
		input_->ReleaseButton(Input::Button::A)) {

		velocity_.y += parameter_.chargeJump_.chargeJumpVelocity;
		parameter_.chargeJump_.canBreak = true;
		parameter_.Jump_.canJump = false;
		isFly_ = true;

	}

	//チャージできたら画像の色変更
	if (parameter_.chargeJump_.chargeTimer == parameter_.chargeJump_.kMaxChargeTime) {
		object_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}
	else {
		object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	if (parameter_.chargeJump_.isCharge && input_->PushButton(Input::Button::A)) {

		//最大値になるまでカウント
		if (parameter_.chargeJump_.chargeTimer < parameter_.chargeJump_.kMaxChargeTime) {
			parameter_.chargeJump_.chargeTimer++;
		}

	}
	else {

		parameter_.chargeJump_.chargeTimer = 0;

	}

}

void Player::WallJump() {

	//壁キック
	if (parameter_.wallJump_.canWallJump && input_->TriggerButton(Input::Button::A)) {

		parameter_.wallJump_.canWallJump = false;

		velocity_.y = parameter_.wallJump_.wallJumpVelocity.y;

		//向きと逆方向に移動する
		if (isFacingLeft_) {
			wallJumpVelocity_.x = parameter_.wallJump_.wallJumpVelocity.x;
		}
		else {
			wallJumpVelocity_.x = -parameter_.wallJump_.wallJumpVelocity.x;
		}

	}

}

void Player::Dig() {

	//地面にいる状態なら穴掘り可能
	if (parameter_.Jump_.canJump && input_->TriggerButton(Input::Button::X)) {

		parameter_.dig_.isDig = true;

		//上下左右どこを掘るか決める(左右優先)
		if (input_->TiltLStick(Input::Stick::Right)) {
			parameter_.dig_.digPosition = { position_.x + Block::kBlockSize_, position_.y };
		}
		else if (input_->TiltLStick(Input::Stick::Left)) {
			parameter_.dig_.digPosition = { position_.x - Block::kBlockSize_, position_.y };
		}
		else if (input_->TiltLStick(Input::Stick::Up)) {
			parameter_.dig_.digPosition = { position_.x, position_.y - Block::kBlockSize_ };
		}
		else if (input_->TiltLStick(Input::Stick::Down)) {
			parameter_.dig_.digPosition = { position_.x, position_.y + Block::kBlockSize_ };
		}

	}
	else {

		parameter_.dig_.isDig = false;

		parameter_.dig_.digPosition = { -10000.0f,-10000.0f };

	}

}

void Player::CheckCollision() {

	Vector2 tmp = position_;

	UpdateGrid();

	parameter_.wallJump_.canWallJump = false;

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

						//破壊可能状態なら押し戻し判定をスキップしブロックを破壊
						if (parameter_.chargeJump_.canBreak && Block::CheckCanBreak(block->GetType())) {
							block->Break();
						}
						else {

							//プレイヤーが左側から侵入したなら右に押し戻し
							if (preLeftTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								velocity_.x = 0;

								SetTmpPosition({ block->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

								//落下に入った時に壁キックを可能にする
								if (velocity_.y > 2.5f) {

									parameter_.wallJump_.canWallJump = true;

								}

							}
							else {

								if (preLeftTop_.x > block->GetPosition().x + Block::kBlockHalfSize_ - 1) {



								}
								else {

									velocity_.y = 0;

									SetTmpPosition({ tmpPosition_.x,block->GetPosition().y + (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

								}

							}

						}

					}
					//右上が当たっていた
					if (IsCollision(block->GetCollision(), rightTop_)) {

						//破壊可能状態なら押し戻し判定をスキップしブロックを破壊
						if (parameter_.chargeJump_.canBreak && Block::CheckCanBreak(block->GetType())) {
							block->Break();
						}
						else {

							//プレイヤーが右側から侵入したなら左に押し戻し
							if (preRightTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								velocity_.x = 0;

								SetTmpPosition({ block->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

								//落下に入った時に壁キックを可能にする
								if (velocity_.y > 2.5f) {

									parameter_.wallJump_.canWallJump = true;

								}

							}
							else {

								if (preRightTop_.x < block->GetPosition().x - Block::kBlockHalfSize_) {



								}
								else {

									velocity_.y = 0;

									SetTmpPosition({ tmpPosition_.x,block->GetPosition().y + (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

								}

							}

						}

					}

					//左下が当たっていた
					if (IsCollision(block->GetCollision(), leftBottom_)) {

						//破壊可能状態なら押し戻し判定をスキップしブロックを破壊
						if (false && Block::CheckCanBreak(block->GetType())) {

						}
						else {

							//プレイヤーが左側から侵入したなら右に押し戻し
							if (preLeftBottom_.y > block->GetPosition().y - Block::kBlockHalfSize_) {

								velocity_.x = 0;

								SetTmpPosition({ block->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

								//落下に入った時に壁キックを可能にする
								if (velocity_.y > 2.5f) {

									parameter_.wallJump_.canWallJump = true;

								}

							}
							else {

								velocity_.y = 0;

								SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

								SetCanJump(true);
								SetIsFly(false);

							}

						}

						

					}

					//右下が当たっていた
					if (IsCollision(block->GetCollision(), rightBottom_)) {

						//破壊可能状態なら押し戻し判定をスキップしブロックを破壊
						if (false && Block::CheckCanBreak(block->GetType())) {

						}
						else {

							//プレイヤーが右側から侵入したなら左に押し戻し
							if (preRightBottom_.y > block->GetPosition().y - Block::kBlockHalfSize_) {

								velocity_.x = 0;

								SetTmpPosition({ block->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

								//落下に入った時に壁キックを可能にする
								if (velocity_.y > 2.5f) {

									parameter_.wallJump_.canWallJump = true;

								}

							}
							else {

								velocity_.y = 0;

								SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

								SetCanJump(true);
								SetIsFly(false);

							}

						}

					}

				}
				else {

					block->SetColor({ 1.0f,1.0f,1.0f,1.0f });

				}

				//ここから穴掘りの当たり判定
				if (parameter_.dig_.isDig && IsCollision(block->GetCollision(), parameter_.dig_.digPosition)) {

					if (Block::CheckCanBreak(block->GetType())) {
						block->Break();
					}

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
