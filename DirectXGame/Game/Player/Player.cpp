#include "Player.h"
#include "TextureManager.h"
#include "imgui.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize() {

	input_ = Input::GetInstance();

	texture_ = TextureManager::GetInstance()->Load("player/player.png");

	position_ = { 200.0f,0.0f };

	object_.reset(Object2d::Create(texture_, position_));
	object_->SetSize({ kPlayerSize_, kPlayerSize_ });

	leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	rightTop_ = { position_.x + kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
	leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };
	rightBottom_ = { position_.x + kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };

	velocity_ = { 0.0f,0.0f };

}

void Player::Update() {

	//ジャンプ処理
	Jump();
	
	//壁キック処理
	WallJump();
	
	//移動処理
	Move();

	//速度加算、オブジェクトに更新した座標を適用
	{

		//浮いている場合の処理
		if (isFly_ && velocity_.y < maxFallSpeed_) {

			velocity_.y += 2.0f;

			//下限値を超えないように調整
			if (velocity_.y > maxFallSpeed_) {
				velocity_.y = maxFallSpeed_;
			}



		}

		//浮いていない場合の処理
		if (!isFly_) {
			
		}

		position_ += velocity_;

		object_->position_ = position_;

		//4頂点の座標を更新
		leftTop_ = { position_.x - kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
		rightTop_ = { position_.x + kPlayerHalfSize_, position_.y - kPlayerHalfSize_ };
		leftBottom_ = { position_.x - kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };
		rightBottom_ = { position_.x + kPlayerHalfSize_, position_.y + kPlayerHalfSize_ };

	}

	//判定処理
	{

		if (leftBottom_.y >= 720.0f) {
			position_.y = 720.0f - kPlayerHalfSize_;
			parameter_.Jump_.canJump = true;
			velocity_.y = 0.0f;
			isFly_ = false;
		}
		else {
			isFly_ = true;
		}

	}

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

	//通常ジャンプ
	if (parameter_.Jump_.canJump && input_->TriggerButton(Input::Button::A)) {
		velocity_.y -= parameter_.Jump_.jumpVelocity;
		parameter_.Jump_.canJump = false;
	}

}

void Player::WallJump() {

	//壁キック
	if (parameter_.wallJump_.canWallJump && input_->TriggerButton(Input::Button::A)) {

	}

}

void Player::Debug() {

#ifdef _DEBUG

	ImGui::Begin("Player");
	ImGui::Text("position x : %1.2f y : %1.2f", position_.x, position_.y);
	ImGui::End();

#endif // _DEBUG

}
