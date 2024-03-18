#include "Player.h"
#include "TextureManager.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize() {

	texture_ = TextureManager::GetInstance()->Load("player/player.png");

	object_.reset(Object2d::Create(texture_, { 200.0f,300.0f }));
	object_->SetSize({ 32.0f,32.0f });

}

void Player::Update() {



}

void Player::Draw(const Camera& camera) {

	object_->Draw(camera);

}
