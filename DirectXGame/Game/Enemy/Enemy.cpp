#include "Enemy.h"

BaseEnemy::BaseEnemy() {
	enemyTexture_ = TextureManager::GetInstance()->Load("enemy/enemy.png");
	object_.reset(Object2d::Create(enemyTexture_, {}));
}

void NormalEnemy::Initialize(const Vector2& position) {

	position_ = position;
	object_->position_ = position_;
	


}

void NormalEnemy::Update() {

}

void NormalEnemy::Draw(const Camera& camera) {

	object_->Draw(camera);

}
