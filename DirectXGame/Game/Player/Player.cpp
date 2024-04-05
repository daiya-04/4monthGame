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

	position_ = { 200.0f,-200.0f };
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

	parameters_[kRightPlayer].Jump_.jumpVelocity = -50.0f;

	characters_ = kLeftPlayer;

	SetOnBase();

}

void Player::SetOnBase() {

	isStopMove_ = true;
	isMining_ = false;
	isReturn_ = false;
	position_ = Stage::kBasePosition;

}

void Player::Update() {

	//前フレームの位置をセット
	prePosition_ = position_;

	preLeftTop_ = leftTop_;
	preRightTop_ = rightTop_;
	preLeftBottom_ = leftBottom_;
	preRightBottom_ = rightBottom_;

	//デバッグ中の移動処理
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

		//採掘中の行動処理
		if (isMining_) {


			//帰還ボーダーに触れたら動きを止める
			if ((position_.y < Stage::kBorderLeft.y || position_.y < Stage::kBorderRight.y) &&
				position_.x > Stage::kBorderLeft.x && position_.x < Stage::kBorderRight.x) {
				isStopMove_ = true;
			}

			//移行できる状態になったら待機モーションに移行
			if (isStopMove_ && parameters_[characters_].Jump_.canJump) {
				isMining_ = false;
				isReturn_ = true;
			}

			//動きを止めるフラグが立ったら操作不可にする
			if (!isStopMove_) {

				//移動処理
				Move();

				//ジャンプ処理
				Jump();

				//壁キック処理
				WallJump();

				//穴掘り処理
				Dig();

			}

			//座標の更新
			UpdatePosition();

			if (characters_ == kLeftPlayer) {
				object_->SetColor({ 1.0f,0.0f,0.0f,1.0f });
			}
			else {
				object_->SetColor({ 0.0f,0.0f,1.0f,1.0f });
			}

		}
		//拠点待機中の処理
		else {

			//キャラクター交代の一連の流れ
			Change();

		}

		
		
	}

	//最終的な当たり判定を更新
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

		velocity_.x += parameters_[characters_].speed_;

		isFacingLeft_ = false;

	}
	else if (input_->TiltLStick(Input::Stick::Left) && wallJumpVelocity_.x <= 1.0f) {

		if (velocity_.x > 2.0f) {
			velocity_.x = 0.0f;
		}

		velocity_.x -= parameters_[characters_].speed_;

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

	//スティック入力で上方向のみの入力の場合にチャージジャンプをできるようにする
	if (parameters_[characters_].Jump_.canJump && input_->TiltLStick(Input::Stick::Up) && 
		!input_->TiltLStick(Input::Stick::Left) && !input_->TiltLStick(Input::Stick::Right)) {

		parameters_[characters_].chargeJump_.isCharge = true;

	}
	else {
		parameters_[characters_].chargeJump_.isCharge = false;
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
		parameters_[characters_].Jump_.canJump = false;
	}

	//通常ジャンプ
	if (parameters_[characters_].Jump_.canJump && input_->TriggerButton(Input::Button::A) && !parameters_[characters_].chargeJump_.isCharge) {
		velocity_.y += parameters_[characters_].Jump_.jumpVelocity;
		parameters_[characters_].Jump_.canJump = false;
	}
	//溜めジャンプが可能なときに必要な溜めの時間まで行かなかったら通常ジャンプの処理に切り替え
	else if (parameters_[characters_].Jump_.canJump && input_->ReleaseButton(Input::Button::A) && parameters_[characters_].chargeJump_.isCharge &&
		parameters_[characters_].chargeJump_.chargeTimer < parameters_[characters_].chargeJump_.kMaxChargeTime) {
		velocity_.y += parameters_[characters_].Jump_.jumpVelocity;
		parameters_[characters_].Jump_.canJump = false;
	}

	//溜めジャンプ処理
	ChargeJump();

}

void Player::ChargeJump() {

	//チャージ完了した状態でボタンを離した時に溜めジャンプ発動
	if (parameters_[characters_].chargeJump_.chargeTimer == parameters_[characters_].chargeJump_.kMaxChargeTime &&
		input_->ReleaseButton(Input::Button::A)) {

		velocity_.y += parameters_[characters_].chargeJump_.chargeJumpVelocity;
		parameters_[characters_].chargeJump_.canBreak = true;
		parameters_[characters_].Jump_.canJump = false;

	}

	//チャージできたら画像の色変更
	if (parameters_[characters_].chargeJump_.chargeTimer == parameters_[characters_].chargeJump_.kMaxChargeTime) {
		object_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}
	else {
		object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	}

	//ジャンプボタンを押している間チャージカウント増加
	if (parameters_[characters_].chargeJump_.isCharge && input_->PushButton(Input::Button::A)) {

		//最大値になるまでカウント
		if (parameters_[characters_].chargeJump_.chargeTimer < parameters_[characters_].chargeJump_.kMaxChargeTime) {
			parameters_[characters_].chargeJump_.chargeTimer++;
		}

	}
	else {

		parameters_[characters_].chargeJump_.chargeTimer = 0;

	}

}

void Player::WallJump() {

	//壁キック
	if (parameters_[characters_].wallJump_.canWallJump && input_->TriggerButton(Input::Button::A)) {

		parameters_[characters_].wallJump_.canWallJump = false;

		velocity_.y = parameters_[characters_].wallJump_.wallJumpVelocity.y;

		//向きと逆方向に移動する
		if (isFacingLeft_) {
			wallJumpVelocity_.x = parameters_[characters_].wallJump_.wallJumpVelocity.x;
		}
		else {
			wallJumpVelocity_.x = -parameters_[characters_].wallJump_.wallJumpVelocity.x;
		}

	}

}

void Player::Dig() {

	//地面にいる状態なら穴掘り可能
	if (parameters_[characters_].Jump_.canJump && input_->TriggerButton(Input::Button::X)) {

		parameters_[characters_].dig_.isDig = true;

		//上下左右どこを掘るか決める(左右優先)
		if (input_->TiltLStick(Input::Stick::Right)) {
			parameters_[characters_].dig_.digPosition = { position_.x + Block::kBlockSize_, position_.y };
		}
		else if (input_->TiltLStick(Input::Stick::Left)) {
			parameters_[characters_].dig_.digPosition = { position_.x - Block::kBlockSize_, position_.y };
		}
		else if (input_->TiltLStick(Input::Stick::Up)) {
			parameters_[characters_].dig_.digPosition = { position_.x, position_.y - Block::kBlockSize_ };
		}
		else if (input_->TiltLStick(Input::Stick::Down)) {
			parameters_[characters_].dig_.digPosition = { position_.x, position_.y + Block::kBlockSize_ };
		}

	}
	else {

		parameters_[characters_].dig_.isDig = false;

		parameters_[characters_].dig_.digPosition = { -10000.0f,-10000.0f };

	}

}

void Player::Enhance() {



}

void Player::Change() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (characters_ < kLeftPlayer || characters_ > kRightPlayer) {
		return;
	}

	//帰還中の挙動
	if (isReturn_) {

		//左側にいた場合
		if (position_.x < Stage::kBasePosition.x) {

			object_->SetTextureHandle(textureRight_);

			position_.x += parameters_[characters_].speed_ * 5.0f;

			//サウナ室内に入ったら切り替え
			if (position_.x > Stage::kBasePosition.x - Block::kBlockHalfSize_) {
				
				if (characters_ == kLeftPlayer) {
					characters_ = kRightPlayer;
				}
				else {
					characters_ = kLeftPlayer;
				}

				position_.x = Stage::kBasePosition.x;
				isReturn_ = false;

			}

		}
		else {

			object_->SetTextureHandle(textureLeft_);

			position_.x -= parameters_[characters_].speed_ * 5.0f;

			//サウナ室内に入ったら切り替え
			if (position_.x < Stage::kBasePosition.x + Block::kBlockHalfSize_) {
				
				if (characters_ == kLeftPlayer) {
					characters_ = kRightPlayer;
				}
				else {
					characters_ = kLeftPlayer;
				}

				position_.x = Stage::kBasePosition.x;
				isReturn_ = false;

			}

		}

	}
	//外に出る途中の挙動
	else if (isOut_) {

		switch (characters_)
		{
		default:
		case Player::kLeftPlayer:

			position_.x -= parameters_[characters_].speed_ * 5.0f;

			//サウナ室内に入ったら切り替え
			if (position_.x < Stage::kBorderLeft.x - Block::kBlockSize_) {
				isOut_ = false;
				isMining_ = true;
				isStopMove_ = false;
			}


			break;
		case Player::kRightPlayer:

			position_.x += parameters_[characters_].speed_ * 5.0f;

			//サウナ室内に入ったら切り替え
			if (position_.x > Stage::kBorderRight.x + Block::kBlockSize_) {
				isOut_ = false;
				isMining_ = true;
				isStopMove_ = false;
			}

			break;
		}

	}
	//拠点にいる時の挙動
	else {

		if (input_->TriggerButton(Input::Button::A)) {
			isOut_ = true;
		}

	}

	object_->position_ = position_;

	//4頂点の座標を更新
	leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	rightTop_ = { position_.x + kPlayerHalfSize_ - 1, position_.y - kPlayerHalfSize_ };
	leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ - 1 };
	rightBottom_ = { position_.x + kPlayerHalfSize_ - 1, position_.y + kPlayerHalfSize_ - 1 };

}

void Player::UpdatePosition() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (characters_ < kLeftPlayer || characters_ > kRightPlayer) {
		return;
	}

	//左右速度の制限
	if (std::fabsf(velocity_.x) > parameters_[characters_].maxMoveSpeed_) {

		if (velocity_.x > 0.0f) {
			velocity_.x = parameters_[characters_].maxMoveSpeed_;
		}
		else {
			velocity_.x = -parameters_[characters_].maxMoveSpeed_;
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
	if (std::fabsf(wallJumpVelocity_.x + velocity_.x) > parameters_[characters_].maxMoveSpeed_) {

		if (wallJumpVelocity_.x + velocity_.x > 0.0f) {
			velocity_.x = parameters_[characters_].maxMoveSpeed_ - wallJumpVelocity_.x;
		}
		else {
			velocity_.x = -parameters_[characters_].maxMoveSpeed_ - wallJumpVelocity_.x;
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
	if (parameters_[characters_].chargeJump_.canBreak && velocity_.y > parameters_[characters_].chargeJump_.unBreakVelocity) {
		parameters_[characters_].chargeJump_.canBreak = false;
	}

	//壁キック可能時(壁ずりしている時)、下方向の落下速度を減少させる
	if (parameters_[characters_].wallJump_.canWallJump) {

		velocity_.y *= 0.6f;

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

void Player::CheckCollision() {

	Vector2 tmp = position_;

	parameters_[characters_].wallJump_.canWallJump = false;

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
						if (parameters_[characters_].chargeJump_.canBreak && Block::CheckCanBreak(block->GetType())) {
							block->Break();
						}
						else {

							//プレイヤーが左側から侵入したなら右に押し戻し
							if (preLeftTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								velocity_.x = 0;

								SetTmpPosition({ block->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

								//落下に入った時に壁キックを可能にする
								if (velocity_.y > 2.5f) {

									parameters_[characters_].wallJump_.canWallJump = true;

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
						if (parameters_[characters_].chargeJump_.canBreak && Block::CheckCanBreak(block->GetType())) {
							block->Break();
						}
						else {

							//プレイヤーが右側から侵入したなら左に押し戻し
							if (preRightTop_.y < block->GetPosition().y + Block::kBlockHalfSize_ - 1) {

								velocity_.x = 0;

								SetTmpPosition({ block->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSize_), tmpPosition_.y });

								//落下に入った時に壁キックを可能にする
								if (velocity_.y > 2.5f) {

									parameters_[characters_].wallJump_.canWallJump = true;

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

									parameters_[characters_].wallJump_.canWallJump = true;

								}

							}
							else {

								velocity_.y = 0;

								SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

								SetCanJump(true);

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

									parameters_[characters_].wallJump_.canWallJump = true;

								}

							}
							else {

								velocity_.y = 0;

								SetTmpPosition({ tmpPosition_.x,block->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSize_) });

								SetCanJump(true);

							}

						}

					}

				}
				else {

					block->SetColor({ 1.0f,1.0f,1.0f,1.0f });

				}

				//ここから穴掘りの当たり判定
				if (parameters_[characters_].dig_.isDig && IsCollision(block->GetCollision(), parameters_[characters_].dig_.digPosition)) {

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

void Player::Debug() {

#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::Text("position x : %1.2f y : %1.2f", position_.x, position_.y);
	ImGui::End();

#endif // _DEBUG

}
