#include "SaunaStone.h"
#include "TextureManager.h"

SaunaStone::SaunaStone()
{
}

SaunaStone::~SaunaStone()
{
}

void SaunaStone::Initialize(const Vector2& position) {

	position_ = position;

	texture_ = TextureManager::GetInstance()->Load("saunaStone/saunaStone.png");

	object_.reset(Object2d::Create(texture_, position_));

}

void SaunaStone::Update() {



}

void SaunaStone::Draw(const Camera& camera) {

	object_->Draw(camera);

}
