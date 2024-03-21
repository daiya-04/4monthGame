#include "Player.h"
#include "TextureManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize() {

	input_ = Input::GetInstance();

	texture_ = TextureManager::GetInstance()->Load("player/player.png");

	object_.reset(Object2d::Create(texture_, { 200.0f,300.0f }));
	object_->SetSize({ 32.0f,32.0f });

}

void Player::Update() {

	velocity_ = { 0.0f,0.0f };

	if (input_->TriggerLStick("Right")) {
		velocity_.x = speed_;
	}

	else if (input_->TriggerLStick("Left")) {
		velocity_.x = -speed_;
	}

	position_ += velocity_;

	object_->position_ = position_;

}

void Player::Draw(const Camera& camera) {

	object_->Draw(camera);

}
