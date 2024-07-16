#include "Block.h"
#include "AudioManager.h"
#include "Player/Player.h"
#include "BlockBreakParticle.h"
#include "GemGetParticle.h"
#include "RandomEngine/RandomEngine.h"
#include "Stage/Magma.h"
#include "EnvironmentEffects/EnvironmentEffectsManager.h"
#include "Enemy/EnemyManager.h"

Score* BaseBlock::score_;

void BaseBlock::Break(float power, bool isPlayer) {

	//耐久値が無い、もしくは0以下ならスキップ
	if (durability_ <= 0.0f) {
		return;
	}

	//耐久値を減少
	durability_ -= power;

	//耐久が0以下で破壊
	if (durability_ <= 0.0f) {

		durability_ = 0.0f;

		if (isPlayer) {
			//穴掘り回数加算
			player_->AddDigCount();
			score_->AddScore(100);
		}

		//タイプに応じてプレイヤーに鉱石を付与
		if (player_ && isPlayer) {
			int32_t num=1;
			if (type_ == kMagma || type_ == kSnow) {
				//player_->AddRockCount(int32_t(defaultDurability_) / 3 + 1);
				num = int32_t(defaultDurability_) / 3 + 1;
			}
			else if (type_ == kBlueBlock) {
				//player_->AddBlueRock();
				crystalSE_->Play();
			}
			else if (type_ == kGreenBlock) {
				//player_->AddGreenRock();
				crystalSE_->Play();
			}
			else if (type_ == kRedBlock) {
				//player_->AddRedRock();
				crystalSE_->Play();
			}
			else if (type_ == kDownMagma) {
				//ラインを1000下げる
				magma_->baseMagmaLine_ += 1000.0f;
				magma_->easingT_ = 0.0f;
				EnvironmentEffectsManager::GetInstance()->MagmaDown(position_);
			}
			else if (type_ == kGoldBlock) {
				player_->SetIsClear(true);
				crystalSE_->Play();
			}
			else if (type_ == kEnemyBlock) {
				EnemyManager::GetInstance()->AddEnemy(position_);
			}

			if (type_ == kMagma || type_ == kSnow || type_ == kRedBlock || type_ == kGreenBlock || type_ == kBlueBlock) {
				GemGetParticle::SetPlayer(player_);
				for (int32_t i = 0; i < num;i++) {
					BlockTextureManager::GetInstance()->CreateGemParticle(position_, type_);
				}
			}
		}
		else if (!isPlayer) {

			//敵が敵の出現ブロックを壊したら出現するようにする
			if (type_ == kEnemyBlock) {
				EnemyManager::GetInstance()->AddEnemy(position_);
			}

		}

		int createNum = int(RandomEngine::GetRandom(8.0f,12.0f));
		for (int i = 0; i < createNum;i++) {
			BlockTextureManager::GetInstance()->CreateParticle(position_, type_);
		}

		isBreak_ = true;
		type_ = kNone;
		//SetColor({ 1.0f,1.0f,1.0f,1.0f });
	
	}
	else {

		if (isPlayer) {
			score_->AddScore(10);
		}

		int createNum = int(RandomEngine::GetRandom(8.0f, 16.0f));
		int fireCreateNum = int(RandomEngine::GetRandom(10.0f, 16.0f));
		Vector2 center = position_;
		Vector2 pCenter = player_->GetPosition();
		Vector2 closestPoint{
			std::clamp(pCenter.x ,center.x - kBlockHalfSize_ ,center.x + kBlockHalfSize_),
			std::clamp(pCenter.y ,center.y - kBlockHalfSize_ ,center.y + kBlockHalfSize_) };
		Vector2 velocity = player_->GetPosition() - center;
		//Vector2 velocity = {0,0};
		Vector2 pos = closestPoint;
		velocity = velocity.Normalize();
		velocity *= 2.0f;
		for (int i = 0; i < createNum; i++) {
			BlockTextureManager::GetInstance()->CreateParticle(pos,velocity, type_);
		}
		velocity = velocity.Normalize();
		velocity *= 0.4f;
		for (int i = 0; i < fireCreateNum; i++) {
			BlockTextureManager::GetInstance()->CreateParticle(pos, velocity, kUnbreakable);
		}

	}

	//耐久力に応じてSE変更
	if (durability_ >= defaultDurability_ - defaultDurability_ / 2.0f) {
		digLowSE_->Play();
	}
	else if (durability_ > 0) {
		digMidSE_->Play();
	}
	else if (durability_ <= 0) {
		digHighSE_->Play();
	}

}

void BaseBlock::UnBreak() {

	int createNum = int(RandomEngine::GetRandom(2.0f, 3.0f));
	Vector2 center = position_;
	Vector2 pCenter = player_->GetPosition();
	Vector2 closestPoint{
		std::clamp(pCenter.x ,center.x - kBlockHalfSize_ ,center.x + kBlockHalfSize_),
		std::clamp(pCenter.y ,center.y - kBlockHalfSize_ ,center.y + kBlockHalfSize_)};
	Vector2 velocity = player_->GetPosition() - center;
	//Vector2 velocity = {0,0};
	Vector2 pos = closestPoint;
	velocity = velocity.Normalize() * 0.3f;
	for (int i = 0; i < createNum; i++) {
		BlockTextureManager::GetInstance()->CreateParticle(pos, velocity, kUnbreakable);
	}

		

	//はじかれるSE入れる


}

void BaseBlock::IceBreak(int32_t breakCoolTime) {

	//氷ブロックはスコア対象外
	if (type_ == kIceBlock) {

		isStartBreak_ = true;
		iceBreakCoolTimer_ = breakCoolTime;

		switch (direction_)
		{
		default:
		case kLeft:

			if (pLeft_ && pLeft_->GetType() == kIceBlock) {
				pLeft_->SetDirection(kLeft);
				pLeft_->IceBreak(breakCoolTime + iceBreakInterval_);
			}

			break;
		case kRight:
			if (pRight_ && pRight_->GetType() == kIceBlock) {
				pRight_->SetDirection(kRight);
				pRight_->IceBreak(breakCoolTime + iceBreakInterval_);
			}
			break;
		case kUp:
			if (pUp_ && pUp_->GetType() == kIceBlock) {
				pUp_->SetDirection(kUp);
				pUp_->IceBreak(breakCoolTime + iceBreakInterval_);
			}
			break;
		case kDown:
			if (pDown_ && pDown_->GetType() == kIceBlock) {
				pDown_->SetDirection(kDown);
				pDown_->IceBreak(breakCoolTime + iceBreakInterval_);
			}
			break;
		}

		return;
	}


}

void BaseBlock::MeltIce() {

	if (type_ == kIceBlock) {
		durability_ = 0.0f;
		type_ = kNone;
		isBreak_ = true;
		int createNum = int(RandomEngine::GetRandom(8.0f, 12.0f));
		for (int i = 0; i < createNum; i++) {
			BlockTextureManager::GetInstance()->CreateParticle(position_, type_);
		}

	}

}

void BaseBlock::SetCollapse() {

	collapseSE_->Play();
	isStartCollapse_ = true;

}

void BaseBlock::Reset() {

	durability_ = 3.0f;

	isStartCollapse_ = false;
	collapseCount_ = maxCollapseCount_;
	isStartBreak_ = false;
	isBreak_ = false;
	SetColor();
	isShining_ = false;
	brightValue_ = 0.0f;

}

Block::Block(const Vector2& position, BlockType type)
{	

	defaultType_ = type;
	type_ = type;
	position_ = position;
	/*object_.reset(Object2d::Create(texture_, position_));*/
	digLowSE_ = AudioManager::GetInstance()->Load("SE/dig_low.mp3");
	digMidSE_ = AudioManager::GetInstance()->Load("SE/dig_mid.mp3");
	digHighSE_ = AudioManager::GetInstance()->Load("SE/dig_high.mp3");
	crystalSE_ = AudioManager::GetInstance()->Load("SE/crystal_get2.mp3");
	collapseSE_ = AudioManager::GetInstance()->Load("SE/collapse.mp3");
	ice1SE_ = AudioManager::GetInstance()->Load("SE/ice_01.mp3");
	ice2SE_ = AudioManager::GetInstance()->Load("SE/ice_02.mp3");
	ice3SE_ = AudioManager::GetInstance()->Load("SE/ice_03.mp3");

}

Block::~Block()
{
}

void Block::Initialize(const Vector2& position, BlockType type) {

	defaultType_ = type;
	type_ = type;
	position_ = position;

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

	isShining_ = false;
	brightValue_ = 0.0f;

	//object_.reset(Object2d::Create(texture_, position_));
	//object_->SetSize({ float(kBlockSize_),float(kBlockSize_) });
	//object_->SetTextureArea({ 0.0f,0.0f }, { kTextureBlockSize_,kTextureBlockSize_ });

}

void Block::Update() {

	if (type_ == kNone) {
		isBreak_ = true;
	}

	if (type_ == kIceBlock) {

		if (isStartBreak_ && --iceBreakCoolTimer_ <= 0) {
			durability_ = 0.0f;
			score_->AddScore(100);
			isBreak_ = true;
			int createNum = int(RandomEngine::GetRandom(8.0f, 12.0f));
			for (int i = 0; i < createNum; i++) {
				BlockTextureManager::GetInstance()->CreateParticle(position_, type_);
			}

			switch (createNum % 3 + 1)
			{
			default:
			case 1:
				ice1SE_->Play();
				break;
			case 2:
				ice2SE_->Play();
				break;
			case 3:
				ice3SE_->Play();
				break;
			}

			type_ = kNone;
			//一旦鉱石で代用
			/*crystalSE_->Play();*/
		}

	}

	//object_->SetTextureArea({ float(uvPositionX_ * kTextureBlockSize_),float(uvPositionY_ * kTextureBlockSize_) },
		//{ kTextureBlockSize_,kTextureBlockSize_ });

	collision_.min = { position_.x - kBlockHalfSize_, position_.y - kBlockHalfSize_ };
	collision_.max = { position_.x + kBlockHalfSize_ - 1, position_.y + kBlockHalfSize_ - 1 };

	//確率でブロックを光らせる
	if (!isShining_ && brightValue_ <= 0.0f && RandomEngine::GetRandom(0.0f, 1.0f) <= kShiningProbability_) {
		isShining_ = true;
	}

	if (isShining_) {

		color_.x += shiningValue_;
		color_.y += shiningValue_;
		color_.z += shiningValue_;
		brightValue_ += shiningValue_;


		if (brightValue_ >= brightLimit_) {
			brightValue_ = brightLimit_;
			isShining_ = false;
		}

	}
	else if(brightValue_ > 0.0f) {

		color_.x -= shiningValue_;
		color_.y -= shiningValue_;
		color_.z -= shiningValue_;
		brightValue_ -= shiningValue_;

		if (brightValue_ <= 0.0f) {
			brightValue_ = 0.0f;
		}

	}

	if (type_ == kGoldBlock) {
		BlockTextureManager::GetInstance()->CreateStarParticle(position_,0);
		BlockTextureManager::GetInstance()->UpdateGoldWaveEffectEffect(position_);
	}

	if (!isBreak_ && type_ != kNone) {
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
			BlockTextureManager::GetInstance()->CreateSandParticle(pos, type_);
		}
	}

	//崩れるカウント
	if (isStartCollapse_ && !isBreak_) {

		if (--collapseCount_ <= 0) {
			durability_ = 0.0f;
			int createNum = int(RandomEngine::GetRandom(8.0f, 12.0f));
			for (int i = 0; i < createNum; i++) {
				BlockTextureManager::GetInstance()->CreateParticle(position_, type_);
			}
			isBreak_ = true;
			type_ = kNone;
			digHighSE_->Play();
		}

	}

}

void Block::Draw(const Camera& camera) {

	if (!isBreak_) {
		//disolve計算
		float disolveValue =  (defaultDurability_ - durability_) / defaultDurability_;
		 
		//切り替わるタイプのブロックだったら両方入れる
		if (type_ == BlockType::kSnow || type_ == BlockType::kMagma) {
			BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, kMagma, color_, disolveValue);
			BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, kSnow, color_, disolveValue);
		}
		else {

			//破壊可能なもののみ色を変える
			/*if (CheckCanBreak(type_) && type_ != kDownMagma && type_ != kGoldBlock) {
				BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, type_, color_, disolveValue);
			}
			else {
				BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, type_, disolveValue);
			}*/
			
			BlockTextureManager::GetInstance()->AppendObject(position_, { float(uvPositionX_ * kTextureBlockSize_), float(uvPositionY_ * kTextureBlockSize_) }, { kTextureBlockSize_,kTextureBlockSize_ }, type_, color_, disolveValue);

		}
	}

}

void BaseBlock::SetColor() {

	//一部のブロックは色を変えない
	if (type_ == kGoldBlock || type_ == kDownMagma || type_ == kIceBlock || !CheckCanBreak(type_)) {
		color_ = { 1.0f,1.0f,1.0f, 1.0f };
		return;
	}

	//色分けを分かりやすくするための変数
	float colorVal = std::fmodf(defaultDurability_ + 6.0f, 10.0f);

	//固さに応じて色を変える
	if (colorVal < 1.0f) {
		//元の色
		color_ = { 1.0f,1.0f,1.0f, 1.0f };
	}
	else if (colorVal < 2.0f) {
		//赤めの色
		color_ = { 1.0f,0.5f,0.5f,  1.0f };
	}
	else if(colorVal < 3.0f) {
		//オレンジっぽい色
		color_ = { 1.0f,1.0f, 0.5f, 1.0f };
	}
	else if (colorVal < 4.0f) {
		//緑っぽい色
		color_ = { 0.5,1.0f, 0.5f, 1.0f };
	}
	else if (colorVal < 5.0f) {
		//水色っぽい
		color_ = { 1.0f,0.5f,1.0f,  1.0f };
	}
	else if (colorVal < 6.0f) {
		//青め
		color_ = { 0.5f,0.5f, 1.0f, 1.0f };
	}
	else if (colorVal < 7.0f) {
		//紫
		color_ = { 1.0f, 0.3f,1.0f, 1.0f };
	}
	else if (colorVal < 8.0f) {
		//暗めの色
		color_ = { 0.5f,0.5f,0.5f,  1.0f };
	}
	else if (colorVal < 9.0f) {

		color_ = { 1.0f, 0.3f,0.3f, 1.0f };
	}
	else {
		color_ = { 0.3f, 0.3f,0.3f, 1.0f };
	}

}
