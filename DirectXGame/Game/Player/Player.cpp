#include "Player.h"
#include "TextureManager.h"
#include "imgui.h"
#include "Game/Block/Block.h"
#include "Scroll/Scroll.h"
#include "GlobalVariables.h"

Player::Player()
{

	texture_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueIdle.png");
	textureUp_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueLookUp.png");
	textureDown_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueLookDown.png");
	textureRun_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueRun.png");
	textureBreakUp_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueBreakUp.png");
	textureBreakDown_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueBreakDown.png");
	textureBreak_[kLeftPlayer] = TextureManager::GetInstance()->Load("player/playerBlueBreak.png");
	texture_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeIdle.png");
	textureUp_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeLookUp.png");
	textureDown_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeLookDown.png");
	textureRun_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeRun.png");
	textureBreakUp_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeBreakUp.png");
	textureBreakDown_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeBreakDown.png");
	textureBreak_[kRightPlayer] = TextureManager::GetInstance()->Load("player/playerOrangeBreak.png");
	numberTexture_ = TextureManager::GetInstance()->Load("UI/number.png");
	rockUITextures_[BringRocks::kRock] = TextureManager::GetInstance()->Load("UI/rock.png");
	rockUITextures_[BringRocks::kBlue] = TextureManager::GetInstance()->Load("UI/speedRock.png");
	rockUITextures_[BringRocks::kGreen] = TextureManager::GetInstance()->Load("UI/digSpeedRock.png");
	rockUITextures_[BringRocks::kRed] = TextureManager::GetInstance()->Load("UI/powerRock.png");

	deadTexture_ = TextureManager::GetInstance()->Load("UI/dead.png");

	deadSprite_.reset(Sprite::Create(deadTexture_, { 640.0f,360.0f }));
	object_.reset(Object2d::Create(texture_[kLeftPlayer], position_));
	object_->SetSize({ kPlayerImageSize_, kPlayerImageSize_ });
	object_->SetTextureArea({ 0.0f,0.0f }, { 160.0f,160.0f });

	//数字リセット
	for (int32_t height = 0; height < BringRocks::kMaxType; height++) {

		for (int32_t i = 0; i < 5; i++) {

			numbers_[height][i].reset(Sprite::Create(numberTexture_, { 100.0f + 32.0f * i , 500.0f + 64.0f * height }));
			numbers_[height][i]->SetSize({ 48.0f,48.0f });
			numbers_[height][i]->SetTextureArea({ 0.0f,0.0f }, { 64.0f,64.0f });

		}

		//岩UIリセット
		rocksUI_[height].reset(Sprite::Create(rockUITextures_[height], { 50.0f, 500.0f + 64.0f * height }));
		rocksUI_[height]->SetSize({ 48.0f,48.0f });

	}

}

Player::~Player()
{
}

void Player::Initialize() {

	input_ = Input::GetInstance();

	scoreManager_ = ScoreManager::GetInstance();

	position_ = Stage::kBasePosition;
	birdsEyePosition_ = position_;
	tmpPosition_ = position_;
	prePosition_ = position_;

	collision_.min = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
	collision_.max = { position_.x + kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ };

	leftTop_ = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
	rightTop_ = { position_.x + kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
	leftBottom_ = { position_.x - kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ };
	rightBottom_ = { position_.x + kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ };

	preLeftTop_ = { prePosition_.x - kPlayerHalfSizeX_, prePosition_.y - kPlayerHalfSizeY_ };
	preRightTop_ = { prePosition_.x + kPlayerHalfSizeX_, prePosition_.y - kPlayerHalfSizeY_ };
	preLeftBottom_ = { prePosition_.x - kPlayerHalfSizeX_, prePosition_.y + kPlayerHalfSizeY_ };
	preRightBottom_ = { prePosition_.x + kPlayerHalfSizeX_, prePosition_.y + kPlayerHalfSizeY_ };

	velocity_ = { 0.0f,0.0f };

	defaultParameter_ = std::make_unique<PlayerParameter>();

	parameters_[kLeftPlayer] = std::make_unique<PlayerParameter>();
	parameters_[kRightPlayer] = std::make_unique<PlayerParameter>();

	parameters_[kLeftPlayer]->Initialize();
	parameters_[kRightPlayer]->Initialize();

	//岩の保有数リセット
	for (int32_t i = 0; i < kMaxPlayer; i++) {

		for (int32_t k = 0; k < BringRocks::kMaxType; k++) {

			rockParameters_[i].rocks_[k] = 0;

		}

	}

	currentCharacters_ = kLeftPlayer;
	reserveCharacters_ = kRightPlayer;

	isClear_ = false;

	SetOnBase();

	std::string groupName = "DefaultParameter";

	GlobalVariables::GetInstance()->CreateGroup(dataName,groupName);

	//加算値調整項目
	GlobalVariables::GetInstance()->AddItem<float>(dataName, groupName, "Add Value - Speed", addValue_.speed);
	GlobalVariables::GetInstance()->AddItem<int32_t>(dataName, groupName, "Add Value - DigSpeed", addValue_.digSpeed);

	//パラメータ調整項目
	GlobalVariables::GetInstance()->AddItem<float>(dataName, groupName, "Move - AccelValue", defaultParameter_->speed_);
	GlobalVariables::GetInstance()->AddItem<float>(dataName, groupName, "Move - MaxSpeed", defaultParameter_->maxMoveSpeed_);
	GlobalVariables::GetInstance()->AddItem<float>(dataName, groupName, "Jump - Velocity", defaultParameter_->Jump_.jumpVelocity);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "WallJump - JumpVelocity", defaultParameter_->wallJump_.wallJumpVelocity);
	GlobalVariables::GetInstance()->AddItem<int32_t>(dataName, groupName, "Dig - Interval", defaultParameter_->dig_.digInterval);
	GlobalVariables::GetInstance()->AddItem<int32_t>(dataName, groupName, "ChargeJump - ChargeTime", defaultParameter_->chargeJump_.maxChargeTime);
	GlobalVariables::GetInstance()->AddItem<float>(dataName, groupName, "ChargeJump - Velocity", defaultParameter_->chargeJump_.chargeJumpVelocity);
	GlobalVariables::GetInstance()->AddItem<int32_t>(dataName, groupName, "ChargeJump - JumpValue", defaultParameter_->chargeJump_.jumpValue);

	LoadParameter();

}

void Player::SetOnBase() {

	isFacingLeft_ = true;
	isStopMove_ = true;
	isMining_ = false;
	isReturn_ = false;
	isBirdsEye_ = false;
	isOut_ = false;
	isHome_ = true;
	position_ = Stage::kBasePosition;
	birdsEyePosition_ = position_;
	isDead_ = false;
	damageTimer_ = 120;

}

void Player::Update() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	//画像を動かす処理
	if (++animationTime_ >= changeFrame_) {

		animationTime_ = 0;

		//設定した最大数に達したらリセット
		if (++currentAnimationNum_ >= maxAnimationNum_) {
			currentAnimationNum_ = 0;
		}

	}

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
		if (isMining_ && !isDead_ && !isClear_) {

			//帰還ボーダーに触れたら動きを止める
			if ((position_.y < Stage::kBorderLeft.y || position_.y < Stage::kBorderRight.y) &&
				position_.x > Stage::kBorderLeft.x && position_.x < Stage::kBorderRight.x) {
				isStopMove_ = true;
			}

			//移行できる状態になったら待機モーションに移行
			if (isStopMove_ && parameters_[currentCharacters_]->Jump_.canJump) {
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

			//制限時間カウント
			CountSaunaTime();

		}
		//死亡時処理
		else if (isDead_ && !isClear_) {

		}
		//拠点待機中の処理
		else {

			//キャラクター交代の一連の流れ
			Change();

			//座標の更新
			UpdatePosition();

		}

		object_->SetSize({ kPlayerImageSize_, kPlayerImageSize_ });
		object_->SetTextureArea({ 160.0f * currentAnimationNum_,0.0f }, { 160.0f,160.0f });
		
	}

	//最終的な当たり判定を更新
	collision_.min = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
	collision_.max = { position_.x + kPlayerHalfSizeX_ - 1, position_.y + kPlayerHalfSizeY_ - 1 };

}

void Player::UpdateUI() {

	//数字の更新
	for (int32_t height = 0; height < BringRocks::kMaxType; height++) {

		for (int32_t i = 0; i < 5; i++) {

			int32_t num = 0;

			int32_t divide = int32_t(std::pow(10, 5 - 1 - i));

			num = rockParameters_[currentCharacters_].rocks_[height] / divide;

			numbers_[height][i]->SetTextureArea({ 64.0f * num, 0.0f }, { 64.0f,64.0f });

		}

	}

}

void Player::Draw(const Camera& camera) {

	object_->Draw(camera);

}

void Player::DrawUI() {

	//持っているブロック数の表示
	for (int32_t height = 0; height < BringRocks::kMaxType; height++) {

		for (int32_t i = 0; i < 5; i++) {

			numbers_[height][i]->Draw();

		}

		rocksUI_[height]->Draw();

	}

	if (isDead_) {
		deadSprite_->Draw();
	}

}

/// <summary>
/// プレイヤーの行動処理関連ここから------------------------------------------------
/// </summary>

void Player::Move() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	//モード切替
	if (input_->TriggerButton(Input::Button::X)) {

		//速度をリセットしてモード切替
		if (moveType_ == kNormal) {

			//地面についている時のみ可能にする
			if (parameters_[currentCharacters_]->Jump_.canJump) {
				parameters_[currentCharacters_]->Jump_.canJump = false;
				velocity_ = { 0.0f,0.0f };
				wallJumpVelocity_ = { 0.0f,0.0f };
				moveType_ = kLine;
			}

		}
		else {

			//止まっている時のみ可能にする
			if (fabsf(velocity_.x) < 0.001f && fabsf(velocity_.y) < 0.001f) {
				velocity_ = { 0.0f,0.0f };
				wallJumpVelocity_ = { 0.0f,0.0f };
				moveType_ = kNormal;
			}

		}

	}

	switch (moveType_)
	{
	default:
	case Player::kNormal:

		//移動
		if (input_->TiltLStick(Input::Stick::Right)/* && wallJumpVelocity_.x >= -1.0f*/) {

			if (velocity_.x < -2.0f) {
				velocity_.x = 0.0f;
			}

			velocity_.x += parameters_[currentCharacters_]->speed_;

			isFacingLeft_ = false;

		}
		else if (input_->TiltLStick(Input::Stick::Left)/* && wallJumpVelocity_.x <= 1.0f*/) {

			if (velocity_.x > 2.0f) {
				velocity_.x = 0.0f;
			}

			velocity_.x -= parameters_[currentCharacters_]->speed_;

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
		if (parameters_[currentCharacters_]->Jump_.canJump && input_->TiltLStick(Input::Stick::Up) &&
			!input_->TiltLStick(Input::Stick::Left) && !input_->TiltLStick(Input::Stick::Right)) {

			parameters_[currentCharacters_]->chargeJump_.isCharge = true;

		}
		else {
			parameters_[currentCharacters_]->chargeJump_.isCharge = false;
		}

		//入力方向に応じて画像変更
		if (parameters_[currentCharacters_]->dig_.digCount <= int32_t(parameters_[currentCharacters_]->dig_.digInterval * 0.5f)) {

			if (input_->TiltLStick(Input::Stick::Left) || input_->TiltLStick(Input::Stick::Right)) {
				object_->SetTextureHandle(textureRun_[currentCharacters_]);
			}
			else if (input_->TiltLStick(Input::Stick::Up)) {
				object_->SetTextureHandle(textureUp_[currentCharacters_]);
			}
			else if (input_->TiltLStick(Input::Stick::Down)) {
				object_->SetTextureHandle(textureDown_[currentCharacters_]);
			}
			else {
				object_->SetTextureHandle(texture_[currentCharacters_]);
			}

		}

		if (isFacingLeft_) {
			object_->SetScale({ 1.0f,1.0f });
		}
		else {
			object_->SetScale({ -1.0f,1.0f });
		}

		object_->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		break;
	case Player::kLine:

		//移動
		if (input_->TriggerLStick(Input::Stick::Right) && fabsf(velocity_.x) < 0.001f && fabsf(velocity_.y) < 0.001f) {

			velocity_.x = parameters_[currentCharacters_]->lineMoveSpeed_;
			isFacingLeft_ = false;
			object_->SetTextureHandle(texture_[currentCharacters_]);

		}
		else if (input_->TriggerLStick(Input::Stick::Left) && fabsf(velocity_.x) < 0.001f && fabsf(velocity_.y) < 0.001f) {

			velocity_.x = -parameters_[currentCharacters_]->lineMoveSpeed_;
			isFacingLeft_ = true;
			object_->SetTextureHandle(texture_[currentCharacters_]);

		}
		else if (input_->TriggerLStick(Input::Stick::Up) && fabsf(velocity_.x) < 0.001f && fabsf(velocity_.y) < 0.001f) {

			velocity_.y = -parameters_[currentCharacters_]->lineMoveSpeed_;
			object_->SetTextureHandle(textureUp_[currentCharacters_]);

		}
		else if (input_->TriggerLStick(Input::Stick::Down) && fabsf(velocity_.x) < 0.001f && fabsf(velocity_.y) < 0.001f) {

			velocity_.y = parameters_[currentCharacters_]->lineMoveSpeed_;
			object_->SetTextureHandle(textureDown_[currentCharacters_]);

		}

		object_->SetColor({ 0.0f,1.0f,1.0f,1.0f });

		break;
	}

}

void Player::Jump() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	//通常移動時のみジャンプ処理
	if (moveType_ == kNormal) {

		//落下速度が0.1f以上あったら空中判定とみなし、ジャンプできなくする
		if (velocity_.y > 0.1f) {
			parameters_[currentCharacters_]->Jump_.canJump = false;
		}

		//通常ジャンプ
		if (parameters_[currentCharacters_]->Jump_.canJump && input_->TriggerButton(Input::Button::A) && !parameters_[currentCharacters_]->chargeJump_.isCharge) {
			velocity_.y += parameters_[currentCharacters_]->Jump_.jumpVelocity;
			parameters_[currentCharacters_]->Jump_.canJump = false;
		}
		//溜めジャンプが可能なときに必要な溜めの時間まで行かなかったら通常ジャンプの処理に切り替え
		else if (parameters_[currentCharacters_]->Jump_.canJump && input_->ReleaseButton(Input::Button::A) && parameters_[currentCharacters_]->chargeJump_.isCharge &&
			parameters_[currentCharacters_]->chargeJump_.chargeTimer < parameters_[currentCharacters_]->chargeJump_.maxChargeTime) {
			velocity_.y += parameters_[currentCharacters_]->Jump_.jumpVelocity;
			parameters_[currentCharacters_]->Jump_.canJump = false;
		}

		//溜めジャンプ処理
		ChargeJump();

	}

}

void Player::ChargeJump() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	//チャージジャンプ中
	if (parameters_[currentCharacters_]->chargeJump_.isChargeJumping) {

		//チャージジャンプ中は、velocityを調整
		/*velocity_.x = 0.0f;*/

		velocity_.y = -8.0f + parameters_[currentCharacters_]->chargeJump_.chargeJumpVelocity * 
			((position_.y - parameters_[currentCharacters_]->chargeJump_.stopLine) / 
				(parameters_[currentCharacters_]->chargeJump_.jumpValue * Block::kBlockSize_ + 1));

		//velocityが一定値を超えたら調整
		if (velocity_.y > -3.0f) {
			velocity_.y = -3.0f;
		}

		if (velocity_.y < parameters_[currentCharacters_]->chargeJump_.chargeJumpVelocity) {
			velocity_.y = parameters_[currentCharacters_]->chargeJump_.chargeJumpVelocity;
		}

		//停止ラインを超えたらジャンプを止める
		if (position_.y < parameters_[currentCharacters_]->chargeJump_.stopLine) {
			parameters_[currentCharacters_]->chargeJump_.isChargeJumping = false;
			velocity_.y = 0.0f;
			parameters_[currentCharacters_]->chargeJump_.canBreak = false;
		}

	}

	//チャージ完了した状態でボタンを離した時に溜めジャンプ発動
	if (parameters_[currentCharacters_]->chargeJump_.chargeTimer == parameters_[currentCharacters_]->chargeJump_.maxChargeTime &&
		input_->ReleaseButton(Input::Button::A)) {

		velocity_.y = parameters_[currentCharacters_]->chargeJump_.chargeJumpVelocity;

		parameters_[currentCharacters_]->chargeJump_.isChargeJumping = true;
		//既定のマス分上の位置を停止ラインに設定
		parameters_[currentCharacters_]->chargeJump_.stopLine = 
			position_.y - float(parameters_[currentCharacters_]->chargeJump_.jumpValue * Block::kBlockSize_);
		parameters_[currentCharacters_]->chargeJump_.canBreak = true;
		parameters_[currentCharacters_]->Jump_.canJump = false;

	}

	//チャージできたら画像の色変更
	if (parameters_[currentCharacters_]->chargeJump_.chargeTimer == parameters_[currentCharacters_]->chargeJump_.maxChargeTime) {
		object_->SetColor({ 1.0f,1.0f,0.0f,1.0f });
	}

	//ジャンプボタンを押している間チャージカウント増加
	if (parameters_[currentCharacters_]->chargeJump_.isCharge && input_->PushButton(Input::Button::A)) {

		//最大値になるまでカウント
		if (parameters_[currentCharacters_]->chargeJump_.chargeTimer < parameters_[currentCharacters_]->chargeJump_.maxChargeTime) {
			parameters_[currentCharacters_]->chargeJump_.chargeTimer++;
		}

	}
	else {

		parameters_[currentCharacters_]->chargeJump_.chargeTimer = 0;

	}

}

void Player::WallJump() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	if (moveType_ == kNormal) {

		//壁キック
		if (parameters_[currentCharacters_]->wallJump_.canWallJump && input_->TriggerButton(Input::Button::A)) {

			parameters_[currentCharacters_]->wallJump_.canWallJump = false;

			velocity_.y = parameters_[currentCharacters_]->wallJump_.wallJumpVelocity.y;

			//向きと逆方向に移動する
			if (isFacingLeft_) {
				wallJumpVelocity_.x = parameters_[currentCharacters_]->wallJump_.wallJumpVelocity.x;
			}
			else {
				wallJumpVelocity_.x = -parameters_[currentCharacters_]->wallJump_.wallJumpVelocity.x;
			}

		}

	}

}

void Player::Dig() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	if (moveType_ == kNormal) {

		//カウント減少
		if (parameters_[currentCharacters_]->dig_.digCount > 0) {
			parameters_[currentCharacters_]->dig_.digCount--;
		}

		//地面にいる状態且つクールタイムが終わったら穴掘り可能
		if (parameters_[currentCharacters_]->Jump_.canJump && input_->PushTrigger(Input::Trigger::Right) &&
			parameters_[currentCharacters_]->dig_.digCount <= 0) {

			parameters_[currentCharacters_]->dig_.isDig = true;

			//穴掘りクールタイムを設定
			parameters_[currentCharacters_]->dig_.digCount = parameters_[currentCharacters_]->dig_.digInterval;

			//上下左右どこを掘るか決める(左右優先)
			if (input_->TiltLStick(Input::Stick::Right)) {
				animationTime_ = 0;
				currentAnimationNum_ = 0;
				object_->SetTextureHandle(textureBreak_[currentCharacters_]);
				parameters_[currentCharacters_]->dig_.digPosition = { position_.x + Block::kBlockSize_, position_.y };
			}
			else if (input_->TiltLStick(Input::Stick::Left)) {
				animationTime_ = 0;
				currentAnimationNum_ = 0;
				object_->SetTextureHandle(textureBreak_[currentCharacters_]);
				parameters_[currentCharacters_]->dig_.digPosition = { position_.x - Block::kBlockSize_, position_.y };
			}
			else if (input_->TiltLStick(Input::Stick::Up)) {
				animationTime_ = 0;
				currentAnimationNum_ = 0;
				object_->SetTextureHandle(textureBreakUp_[currentCharacters_]);
				parameters_[currentCharacters_]->dig_.digPosition = { position_.x, position_.y - Block::kBlockSize_ };
			}
			else if (input_->TiltLStick(Input::Stick::Down)) {
				animationTime_ = 0;
				currentAnimationNum_ = 0;
				object_->SetTextureHandle(textureBreakDown_[currentCharacters_]);
				parameters_[currentCharacters_]->dig_.digPosition = { position_.x, position_.y + Block::kBlockSize_ };
			}

		}
		else {

			parameters_[currentCharacters_]->dig_.isDig = false;

			parameters_[currentCharacters_]->dig_.digPosition = { -10000.0f,-10000.0f };

		}

	}

}

void Player::Enhance() {



}

/// <summary>
/// プレイヤーの帰還、外出関連処理ここから------------------------------------------------
/// </summary>

void Player::MoveLift() {

	//地面についている時のみ反応する
	if (input_->TriggerButton(Input::Button::A) && parameters_[currentCharacters_]->Jump_.canJump) {

		//左側の場合
		if (position_.x < Stage::kBasePosition.x) {

			position_.x = 13.5f * Block::kBlockSize_;
			position_.y = -1.0f * Block::kBlockSize_;

		}
		//右側の場合
		else {

			position_.x = 25.5f * Block::kBlockSize_;
			position_.y = -1.0f * Block::kBlockSize_;

		}

	}

}

void Player::Change() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	//帰還中の挙動
	if (isReturn_) {

		//左側にいた場合
		if (position_.x < Stage::kBasePosition.x) {

			object_->SetTextureHandle(texture_[currentCharacters_]);

			velocity_.x += parameters_[currentCharacters_]->speed_ * 10.0f;

			

		}
		else {

			object_->SetTextureHandle(texture_[currentCharacters_]);

			velocity_.x -= parameters_[currentCharacters_]->speed_ * 10.0f;

		}

		//サウナ室内に入ったら切り替え
		if (position_.x > Stage::kBasePosition.x - Block::kBlockHalfSize_ && 
			position_.x < Stage::kBasePosition.x + Block::kBlockHalfSize_) {

			//チャージジャンプ止め
			parameters_[currentCharacters_]->chargeJump_.isChargeJumping = false;
			parameters_[currentCharacters_]->chargeJump_.canBreak = false;

			//プレイヤー切り替え
			if (currentCharacters_ == kLeftPlayer) {
				currentCharacters_ = kRightPlayer;
				reserveCharacters_ = kLeftPlayer;
			}
			else {
				currentCharacters_ = kLeftPlayer;
				reserveCharacters_ = kRightPlayer;
			}

			position_.x = Stage::kBasePosition.x;
			velocity_ = { 0.0f,0.0f };
			isReturn_ = false;

		}

	}
	//外に出る途中の挙動
	else if (isOut_) {

		switch (currentCharacters_)
		{
		default:
		case Player::kLeftPlayer:

			object_->SetTextureHandle(texture_[currentCharacters_]);

			velocity_.x -= parameters_[currentCharacters_]->speed_ * 10.0f;

			//ジャンプを入れる
			if (parameters_[currentCharacters_]->Jump_.canJump &&
				position_.x < Stage::kBorderLeft.x - Block::kBlockSize_) {
				velocity_.y += parameters_[currentCharacters_]->Jump_.jumpVelocity;
				parameters_[currentCharacters_]->Jump_.canJump = false;
			}

			//ラインを超えたら採掘時の処理ループに移行
			if (position_.x < Stage::kBorderLeft.x - Block::kBlockSize_ * 4) {
				velocity_.x = 0.0f;
				isOut_ = false;
				isMining_ = true;
				isStopMove_ = false;
			}


			break;
		case Player::kRightPlayer:

			object_->SetTextureHandle(texture_[currentCharacters_]);

			velocity_.x += parameters_[currentCharacters_]->speed_ * 10.0f;

			//ジャンプを入れる
			if (parameters_[currentCharacters_]->Jump_.canJump &&
				position_.x > Stage::kBorderRight.x + Block::kBlockSize_) {
				velocity_.y += parameters_[currentCharacters_]->Jump_.jumpVelocity;
				parameters_[currentCharacters_]->Jump_.canJump = false;
			}

			//ラインを超えたら採掘時の処理ループに移行
			if (position_.x > Stage::kBorderRight.x + Block::kBlockSize_ * 4) {
				velocity_.x = 0.0f;
				isOut_ = false;
				isMining_ = true;
				isStopMove_ = false;
			}

			break;
		}

	}
	//俯瞰視点時の行動
	else if (isBirdsEye_) {

		//カメラのターゲット座標移動
		if (input_->TiltLStick(Input::Stick::Right)) {

			birdsEyePosition_.x += 20.0f;

		}
		else if (input_->TiltLStick(Input::Stick::Left)) {

			birdsEyePosition_.x -= 20.0f;

		}

		if (input_->TiltLStick(Input::Stick::Up)) {

			birdsEyePosition_.y -= 20.0f;

		}
		else if (input_->TiltLStick(Input::Stick::Down)) {

			birdsEyePosition_.y += 20.0f;

		}

		birdsEyePosition_.x = std::clamp(birdsEyePosition_.x, 0.0f + Scroll::kWindowCenter_.x, Scroll::limitEnd_.x);
		birdsEyePosition_.y = std::clamp(birdsEyePosition_.y, -80.0f, Scroll::limitEnd_.y);

		if (input_->TriggerButton(Input::Button::X)) {
			birdsEyePosition_ = position_;
			isBirdsEye_ = false;
		}

	}
	//拠点にいる時の挙動
	else {

		//拠点にいるフラグを立てる
		isHome_ = true;

		if (!isClear_) {

			if (input_->TriggerButton(Input::Button::A)) {
				isOut_ = true;
				isHome_ = false;
			}
			else if (input_->TriggerButton(Input::Button::X)) {
				birdsEyePosition_ = position_;
				isBirdsEye_ = true;
			}

		}

	}

	object_->position_ = position_;

	//4頂点の座標を更新
	leftTop_ = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeY_ };
	rightTop_ = { position_.x + kPlayerHalfSizeX_ - 1, position_.y - kPlayerHalfSizeY_ };
	leftBottom_ = { position_.x - kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeY_ - 1 };
	rightBottom_ = { position_.x + kPlayerHalfSizeX_ - 1, position_.y + kPlayerHalfSizeY_ - 1 };

}

void Player::CountSaunaTime() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

}

/// <summary>
/// プレイヤーの位置移動関連ここから------------------------------------------------
/// </summary>

void Player::UpdatePosition() {

	//範囲外参照を防ぐため、characters_の値がどちらでもない場合、return
	if (currentCharacters_ < kLeftPlayer || currentCharacters_ > kRightPlayer) {
		return;
	}

	switch (moveType_)
	{
	default:
	case Player::kNormal:

		//左右速度の制限
		if (std::fabsf(velocity_.x) > parameters_[currentCharacters_]->maxMoveSpeed_) {

			if (velocity_.x > 0.0f) {
				velocity_.x = parameters_[currentCharacters_]->maxMoveSpeed_;
			}
			else {
				velocity_.x = -parameters_[currentCharacters_]->maxMoveSpeed_;
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
		if (std::fabsf(wallJumpVelocity_.x + velocity_.x) > parameters_[currentCharacters_]->maxMoveSpeed_) {

			if (wallJumpVelocity_.x + velocity_.x > 0.0f) {
				velocity_.x = parameters_[currentCharacters_]->maxMoveSpeed_ - wallJumpVelocity_.x;
			}
			else {
				velocity_.x = -parameters_[currentCharacters_]->maxMoveSpeed_ - wallJumpVelocity_.x;
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

		//壁キック可能時(壁ずりしている時)、下方向の落下速度を減少させる
		if (parameters_[currentCharacters_]->wallJump_.canWallJump) {

			velocity_.y *= 0.6f;

		}

		break;
	case Player::kLine:

		//規定の高さを超えると強制的に解除
		if (position_.y < Block::kBlockSize_ * 5.0f) {
			velocity_ = { 0.0f,0.0f };
			wallJumpVelocity_ = { 0.0f,0.0f };
			moveType_ = kNormal;
		}

		break;
	}

	position_.x = std::clamp(position_.x, float(Block::kBlockHalfSize_ * 0.0f), float(Block::kBlockSize_ * (Stage::kMaxStageWidth_ - 1)));

	tmpPosition_ = position_;

	tmpPosition_ += velocity_ + wallJumpVelocity_;

	//4頂点の座標を更新
	leftTop_ = { tmpPosition_.x - kPlayerHalfSizeX_, tmpPosition_.y - kPlayerHalfSizeY_ };
	rightTop_ = { tmpPosition_.x + kPlayerHalfSizeX_ - 1, tmpPosition_.y - kPlayerHalfSizeY_ };
	leftBottom_ = { tmpPosition_.x - kPlayerHalfSizeX_, tmpPosition_.y + kPlayerHalfSizeY_ - 1 };
	rightBottom_ = { tmpPosition_.x + kPlayerHalfSizeX_ - 1, tmpPosition_.y + kPlayerHalfSizeY_ - 1 };

	//仮の当たり判定更新
	collision_.min = { tmpPosition_.x - kPlayerHalfSizeX_, tmpPosition_.y - kPlayerHalfSizeY_ };
	collision_.max = { tmpPosition_.x + kPlayerHalfSizeX_ - 1, tmpPosition_.y + kPlayerHalfSizeY_ - 1 };

	CheckCollision();

	position_ = tmpPosition_;

	object_->position_ = position_;

	//4頂点の座標を更新
	leftTop_ = { position_.x - kPlayerHalfSizeX_, position_.y - kPlayerHalfSizeX_ };
	rightTop_ = { position_.x + kPlayerHalfSizeX_ - 1, position_.y - kPlayerHalfSizeX_ };
	leftBottom_ = { position_.x - kPlayerHalfSizeX_, position_.y + kPlayerHalfSizeX_ - 1 };
	rightBottom_ = { position_.x + kPlayerHalfSizeX_ - 1, position_.y + kPlayerHalfSizeX_ - 1 };

}

void Player::DamageUpdate() {

	//一定間隔ごとにダメージを受ける
	if (--damageTimer_ <= 0) {

		damageTimer_ = 120;

		for (int32_t i = 0; i < BringRocks::kMaxType; i++) {

			//1つ以上持っていたら約10%減少させる
			if (rockParameters_[currentCharacters_].rocks_[i] > 1) {

				rockParameters_[currentCharacters_].rocks_[i] -= (rockParameters_[currentCharacters_].rocks_[i] / 10) + 1;

				//0未満になってしまったら調整
				if (rockParameters_[currentCharacters_].rocks_[i] < 0) {
					rockParameters_[currentCharacters_].rocks_[i] = 0;
				}

			}

		}

	}

}

void Player::HealUpdate() {

	//ダメージを受ける間隔を回復させる
	if (damageTimer_ < 120) {
		damageTimer_++;
	}

}

void Player::CheckCollision() {

	parameters_[currentCharacters_]->wallJump_.canWallJump = false;

	if (blocksPtr_) {

		for (uint32_t y = 0; y < Stage::kMaxStageHeight_; y++) {

			for (uint32_t x = 0; x < Stage::kMaxStageWidth_; x++) {

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

						if ((*blocksPtr_)[y][x]->isDebug_) {

							//(*blocksPtr_)[y][x]->SetColor({ 1.0f,0.0f,0.0f,1.0f });

						}

						//左上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftTop_)) {

							//破壊可能状態なら押し戻し判定をスキップしブロックを破壊
							if (parameters_[currentCharacters_]->chargeJump_.canBreak && Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType())) {
								(*blocksPtr_)[y][x]->Break(999);
							}
							else {

								//プレイヤーが右側から侵入したなら右に押し戻し
								if (preLeftTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

									SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSizeX_), tmpPosition_.y });

									switch (moveType_)
									{
									default:
									case Player::kNormal:

										velocity_.x = 0;
										wallJumpVelocity_.x = 0.0f;

										//落下に入った時に壁キックを可能にする
										if (velocity_.y > 2.5f) {

											parameters_[currentCharacters_]->wallJump_.canWallJump = true;

										}

										break;
									case Player::kLine:

										velocity_ = { 0.0f,0.0f };

										break;
									}

								}
								else {

									if (preLeftTop_.x > (*blocksPtr_)[y][x]->GetPosition().x + Block::kBlockHalfSize_ - 1) {



									}
									else {

										SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kPlayerHalfSizeY_) });

										switch (moveType_)
										{
										default:
										case Player::kNormal:

											parameters_[currentCharacters_]->chargeJump_.isChargeJumping = false;
											parameters_[currentCharacters_]->chargeJump_.canBreak = false;

											velocity_.y = 0;

											break;
										case Player::kLine:

											velocity_ = { 0.0f,0.0f };

											break;
										}

									}

								}

							}

						}
						//右上が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightTop_)) {

							//破壊可能状態なら押し戻し判定をスキップしブロックを破壊
							if (parameters_[currentCharacters_]->chargeJump_.canBreak && Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType())) {
								(*blocksPtr_)[y][x]->Break(999);
							}
							else {

								//プレイヤーが左側から侵入したなら左に押し戻し
								if (preRightTop_.y < (*blocksPtr_)[y][x]->GetPosition().y + Block::kBlockHalfSize_ - 1) {

									SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSizeX_), tmpPosition_.y });

									switch (moveType_)
									{
									default:
									case Player::kNormal:

										velocity_.x = 0;
										wallJumpVelocity_.x = 0.0f;

										//落下に入った時に壁キックを可能にする
										if (velocity_.y > 2.5f) {

											parameters_[currentCharacters_]->wallJump_.canWallJump = true;

										}

										break;
									case Player::kLine:

										velocity_ = { 0.0f,0.0f };

										break;
									}

								}
								else {

									if (preRightTop_.x < (*blocksPtr_)[y][x]->GetPosition().x - Block::kBlockHalfSize_) {



									}
									else {

										SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y + (Block::kBlockHalfSize_ + kPlayerHalfSizeY_) });

										switch (moveType_)
										{
										default:
										case Player::kNormal:

											parameters_[currentCharacters_]->chargeJump_.isChargeJumping = false;
											parameters_[currentCharacters_]->chargeJump_.canBreak = false;

											velocity_.y = 0;

											break;
										case Player::kLine:

											velocity_ = { 0.0f,0.0f };

											break;
										}

									}

								}

							}

						}

						//左下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), leftBottom_)) {

							//プレイヤーが右側から侵入したなら右に押し戻し
							if (preLeftBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x + (Block::kBlockHalfSize_ + kPlayerHalfSizeX_), tmpPosition_.y });

								switch (moveType_)
								{
								default:
								case Player::kNormal:

									velocity_.x = 0;
									wallJumpVelocity_.x = 0.0f;

									//落下に入った時に壁キックを可能にする
									if (velocity_.y > 2.5f) {

										parameters_[currentCharacters_]->wallJump_.canWallJump = true;

									}

									break;
								case Player::kLine:

									velocity_ = { 0.0f,0.0f };

									break;
								}

							}
							else {

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSizeY_) });

								switch (moveType_)
								{
								default:
								case Player::kNormal:

									velocity_.y = 0;

									//着地した瞬間
									if (!parameters_[currentCharacters_]->Jump_.canJump) {
										SetCanJump(true);
									}

									break;
								case Player::kLine:

									velocity_ = { 0.0f,0.0f };

									break;
								}

							}

						}

						//右下が当たっていた
						if (IsCollision((*blocksPtr_)[y][x]->GetCollision(), rightBottom_)) {

							//プレイヤーが右側から侵入したなら左に押し戻し
							if (preRightBottom_.y > (*blocksPtr_)[y][x]->GetPosition().y - Block::kBlockHalfSize_) {

								SetTmpPosition({ (*blocksPtr_)[y][x]->GetPosition().x - (Block::kBlockHalfSize_ + kPlayerHalfSizeX_), tmpPosition_.y });

								switch (moveType_)
								{
								default:
								case Player::kNormal:

									velocity_.x = 0;
									wallJumpVelocity_.x = 0.0f;

									//落下に入った時に壁キックを可能にする
									if (velocity_.y > 2.5f) {

										parameters_[currentCharacters_]->wallJump_.canWallJump = true;

									}

									break;
								case Player::kLine:

									velocity_ = { 0.0f,0.0f };

									break;
								}

							}
							else {

								

								SetTmpPosition({ tmpPosition_.x,(*blocksPtr_)[y][x]->GetPosition().y - (Block::kBlockHalfSize_ + kPlayerHalfSizeY_) });

								switch (moveType_)
								{
								default:
								case Player::kNormal:

									velocity_.y = 0;

									//着地した瞬間
									if (!parameters_[currentCharacters_]->Jump_.canJump) {
										SetCanJump(true);
									}

									break;
								case Player::kLine:

									velocity_ = { 0.0f,0.0f };

									break;
								}

							}

						}

					}
					else {

						//(*blocksPtr_)[y][x]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

					}

					//ここから穴掘りの当たり判定
					if (parameters_[currentCharacters_]->dig_.isDig && IsCollision((*blocksPtr_)[y][x]->GetCollision(), parameters_[currentCharacters_]->dig_.digPosition)) {

						if (Block::CheckCanBreak((*blocksPtr_)[y][x]->GetType())) {

							//自身の破壊力に応じてダメージを与える
							(*blocksPtr_)[y][x]->Break(parameters_[currentCharacters_]->dig_.digPower);

							//穴掘りクールタイムを設定
							parameters_[currentCharacters_]->dig_.digCount = parameters_[currentCharacters_]->dig_.digInterval;

						}

					}

				}
				else {

					//(*blocksPtr_)[y][x]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

				}

			}

		}		

	}

}

/// <summary>
/// デバッグ処理関連ここから------------------------------------------------
/// </summary>

void Player::Debug() {

#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::Text("position x : %1.2f y : %1.2f", position_.x, position_.y);

	//デフォルトパラメータを適用
	if (ImGui::Button("Apply Default")) {
		parameters_[kLeftPlayer]->CopyParameter(*defaultParameter_);
		parameters_[kRightPlayer]->CopyParameter(*defaultParameter_);
	}

	ImGui::Separator();
	//デフォルトのパラメータ
	ImGui::Text("Default");
	ImGui::Text("D-Accel : %1.2f", defaultParameter_->speed_);
	ImGui::Text("D-MaxSpeed : %1.2f", defaultParameter_->maxMoveSpeed_);
	ImGui::Text("D-DigInterval : %d", defaultParameter_->dig_.digInterval);
	ImGui::Text("D-DigCoolTime : %d", defaultParameter_->dig_.digCount);
	ImGui::Text("D-DigPower : %d", defaultParameter_->dig_.digPower);
	ImGui::Text("D-MaxChargeTime : %d", defaultParameter_->chargeJump_.maxChargeTime);
	ImGui::Text("D-ChargeTimer : %d", defaultParameter_->chargeJump_.chargeTimer);
	ImGui::Separator();
	//左のプレイヤーのパラメータ
	ImGui::Text("Left");
	ImGui::Text("L-Accel : %1.2f", parameters_[kLeftPlayer]->speed_);
	ImGui::Text("L-MaxSpeed : %1.2f", parameters_[kLeftPlayer]->maxMoveSpeed_);
	ImGui::Text("L-DigInterval : %d", parameters_[kLeftPlayer]->dig_.digInterval);
	ImGui::Text("L-DigCoolTime : %d", parameters_[kLeftPlayer]->dig_.digCount);
	ImGui::Text("L-DigPower : %d", parameters_[kLeftPlayer]->dig_.digPower);
	ImGui::Text("L-MaxChargeTime : %d", parameters_[kLeftPlayer]->chargeJump_.maxChargeTime);
	ImGui::Text("L-ChargeTimer : %d", parameters_[kLeftPlayer]->chargeJump_.chargeTimer);
	ImGui::Separator();
	//右のプレイヤーのパラメータ
	ImGui::Text("Right");
	ImGui::Text("R-Accel : %1.2f", parameters_[kRightPlayer]->speed_);
	ImGui::Text("R-MaxSpeed : %1.2f", parameters_[kRightPlayer]->maxMoveSpeed_);
	ImGui::Text("R-DigInterval : %d", parameters_[kRightPlayer]->dig_.digInterval);
	ImGui::Text("R-DigCoolTime : %d", parameters_[kRightPlayer]->dig_.digCount);
	ImGui::Text("R-DigPower : %d", parameters_[kRightPlayer]->dig_.digPower);
	ImGui::Text("R-MaxChargeTime : %d", parameters_[kRightPlayer]->chargeJump_.maxChargeTime);
	ImGui::Text("R-ChargeTimer : %d", parameters_[kRightPlayer]->chargeJump_.chargeTimer);
	ImGui::End();

	//プレイヤーパラメータ調整
	std::string groupName = "DefaultParameter";

	addValue_.speed = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Add Value - Speed");
	addValue_.digSpeed = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "Add Value - DigSpeed");
	defaultParameter_->speed_ = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Move - AccelValue");
	defaultParameter_->maxMoveSpeed_ = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Move - MaxSpeed");
	defaultParameter_->Jump_.jumpVelocity = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Jump - Velocity");
	defaultParameter_->wallJump_.wallJumpVelocity = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "WallJump - JumpVelocity");
	defaultParameter_->dig_.digInterval = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "Dig - Interval");
	defaultParameter_->chargeJump_.maxChargeTime = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "ChargeJump - ChargeTime");
	defaultParameter_->chargeJump_.chargeJumpVelocity = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "ChargeJump - Velocity");
	defaultParameter_->chargeJump_.jumpValue = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "ChargeJump - JumpValue");

	

#endif // _DEBUG

}

void Player::LoadParameter() {

	//プレイヤーパラメータ調整
	std::string groupName = "DefaultParameter";

	defaultParameter_->speed_ = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Move - AccelValue");
	defaultParameter_->maxMoveSpeed_ = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Move - MaxSpeed");
	defaultParameter_->Jump_.jumpVelocity = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "Jump - Velocity");
	defaultParameter_->wallJump_.wallJumpVelocity = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "WallJump - JumpVelocity");
	defaultParameter_->dig_.digInterval = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "Dig - Interval");
	defaultParameter_->chargeJump_.maxChargeTime = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "ChargeJump - ChargeTime");
	defaultParameter_->chargeJump_.chargeJumpVelocity = GlobalVariables::GetInstance()->GetValue<float>(dataName, groupName, "ChargeJump - Velocity");
	defaultParameter_->chargeJump_.jumpValue = GlobalVariables::GetInstance()->GetValue<int32_t>(dataName, groupName, "ChargeJump - JumpValue");

	parameters_[kLeftPlayer]->CopyParameter(*defaultParameter_);
	parameters_[kRightPlayer]->CopyParameter(*defaultParameter_);

}
