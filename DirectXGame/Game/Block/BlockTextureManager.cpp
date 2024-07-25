#include "BlockTextureManager.h"
#include "TextureManager.h"
#include "Block.h"
#include "BlockBreakParticle.h"
#include "starParticle.h"
#include "SandParticle.h"
#include "GemGetParticle.h"
#include "WallKickEffect.h"
#include "hitEffect.h"
#include "JumpChargeParticle.h"
#include "DirectXCommon.h"
#include "RandomEngine/RandomEngine.h"
BlockTextureManager* BlockTextureManager::GetInstance() {
	static BlockTextureManager instance;
	return &instance;
}

void BlockTextureManager::LoadAllBlockTexture() {

	//マップチップに使うテクスチャを一括でロードする
	blockTextures_.clear();

	uint32_t texture = 0;
	texture = TextureManager::Load("blocks/rocks.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/blue.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/green.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/red.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/hot.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/ice.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/unbreakable.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/gold.png");
	blockTextures_.push_back(texture);
	texture = TextureManager::Load("blocks/testIce.png");
	blockTextures_.push_back(texture);

	for (uint32_t index=1; index < BaseBlock::BlockType::kMaxBlock; index++) {

		if (index == Block::kBlueBlock) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[1]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/rockMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(TextureManager::Load("blocks/rockParticle.png"));
		}
		else if (index == Block::kGreenBlock) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[2]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/rockMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(TextureManager::Load("blocks/rockParticle.png"));
		}
		else if (index == Block::kRedBlock) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[3]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/rockMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(TextureManager::Load("blocks/rockParticle.png"));
		}
		else if (index == Block::kDownMagma) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[5]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/iceMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(blockTextures_[5]);
		}
		else if (index == Block::kUnbreakable || index == Block::kFlagBlock) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[6]);
			breakParticles_[index - 1]->SetTextureHandle(TextureManager::Load("blocks/fire.png"));//ここ火花に差し替える
		}
		else if (index == Block::kGoldBlock) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[7]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/rockMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(blockTextures_[7]);
		}
		else if (index == Block::kIceBlock) {
			objects_[index - 1]->SetTextureHandle(blockTextures_[8]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/iceMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(TextureManager::Load("blocks/iceParticle.png"));
		}
		else {
			objects_[index - 1]->SetTextureHandle(blockTextures_[0]);
			objects_[index - 1]->SetMaskTextureHandle(TextureManager::Load("blocks/rockMask.png"));
			breakParticles_[index - 1]->SetTextureHandle(TextureManager::Load("blocks/rockParticle.png"));
		}

	}

	//壊せないブロック
	/*objects_[Block::kUnbreakable - 1]->SetColor({ 0.6f,0.9f,1.8f,1.0f });*/
	//極寒
	objects_[Block::kSnow - 1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	//灼熱
	objects_[Block::kMagma - 1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	////氷
	//objects_[Block::kIceBlock - 1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	////速度
	//objects_[Block::kBlueBlock - 1]->SetColor({ 0.1f,0.1f,0.3f,1.0f });
	////採掘速度
	//objects_[Block::kGreenBlock - 1]->SetColor({ 0.1f,0.3f,0.1f,1.0f });
	////パワーーー！！！
	//objects_[Block::kRedBlock - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });
	//マグマ下降
	/*objects_[Block::kDownMagma - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });*/
	//黄金ブロック
	/*objects_[Block::kGoldBlock - 1]->SetColor({ 2.0f,2.0f, 0.0f,1.0f });*/

	//壊せないブロック
	/*breakParticles_[Block::kUnbreakable - 1]->SetColor({ 0.6f,0.9f,1.8f,1.0f });*/
	//極寒
	breakParticles_[Block::kSnow - 1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	//灼熱
	breakParticles_[Block::kMagma - 1]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
	////氷
	//breakParticles_[Block::kIceBlock - 1]->SetColor({ 0.7f,0.7f,1.0f,1.0f });
	////速度
	//breakParticles_[Block::kBlueBlock - 1]->SetColor({ 0.1f,0.1f,0.3f,1.0f });
	////採掘速度
	//breakParticles_[Block::kGreenBlock - 1]->SetColor({ 0.1f,0.3f,0.1f,1.0f });
	////パワーーー！！！
	//breakParticles_[Block::kRedBlock - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });
	//マグマ下降
	/*breakParticles_[Block::kDownMagma - 1]->SetColor({ 0.3f,0.1f,0.1f,1.0f });*/
	//黄金ブロック
	/*breakParticles_[Block::kGoldBlock - 1]->SetColor({ 2.0f,2.0f, 0.0f,1.0f });*/


	breakParticleDatas_.clear();
	//star
	starParticleDatas_.clear();
	//sand
	sandParticleDatas_.clear();
	//gem
	gemParticleDatas_.clear();
	//starUI
	starParticleDatasUI_.clear();
	//wallKick
	wallKickEffectDatas_.clear();

	jumpChargeParticleDatas_.clear();

	hitEffectDatas_.clear();

	ClearObject();
}

BlockTextureManager::BlockTextureManager() {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		std::unique_ptr<Object2dInstancing> object;
		object.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 512));
		object->SetSize({ float(BaseBlock::kBlockSize_),float(BaseBlock::kBlockSize_) });
		objects_.push_back(std::move(object));
		object.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 512));
		int32_t size = 4;
		
		if (index == 2 || index == 3 || index == 5 || index == 6 || index == 7){
			size = 2;
		}
		if (index == 4) {
			size = 2;
		}
		object->SetSize({ float(BaseBlock::kBlockSize_ / size),float(BaseBlock::kBlockSize_ / size) });
		breakParticles_.push_back(std::move(object));
	}

	//particle
	starParticles_.reset(Object2dInstancing::Create(TextureManager::Load("goldStar.png"), Vector2{ 0,0 }, 256));
	starParticles_->SetScale({ 1.0f, 1.0f });
	starParticles_->SetSize({ float(BaseBlock::kBlockSize_*0.75f),float(BaseBlock::kBlockSize_*0.75f) });

	sandParticles_.reset(Object2dInstancing::Create(TextureManager::Load("sandDust.png"), Vector2{ 0,0 }, 1024));
	sandParticles_->SetScale({ 1.0f, 1.0f });
	sandParticles_->SetSize({ float(BaseBlock::kBlockSize_ /4),float(BaseBlock::kBlockSize_/4) });

	gemParticles_.reset(Object2dInstancing::Create(TextureManager::Load("gemGetParticle.png"), Vector2{ 0,0 }, 32));
	gemParticles_->SetScale({ 1.0f, 1.0f });
	gemParticles_->SetSize({ float(BaseBlock::kBlockSize_ ),float(BaseBlock::kBlockSize_ ) });

	starParticlesUI_.reset(Object2dInstancing::Create(TextureManager::Load("goldStar.png"), Vector2{ 0,0 }, 256));
	starParticlesUI_->SetScale({ 1.0f, 1.0f });
	starParticlesUI_->SetSize({ float(BaseBlock::kBlockSize_ * 0.4f),float(BaseBlock::kBlockSize_ * 0.4f) });
	constantCamera_.reset(new Camera);
	constantCamera_->Init();
	constantCamera_->ChangeDrawingRange({ 1280.0f,720.0f });
	constantCamera_->UpdateMatrix();

	hitEffects_.reset(Object2dInstancing::Create(TextureManager::Load("goldStar.png"), Vector2{ 0,0 }, 256));
	hitEffects_->SetScale({ 1.0f, 1.0f });
	hitEffects_->SetSize({ float(BaseBlock::kBlockSize_)*0.5f,float(BaseBlock::kBlockSize_)*30.0f });

	//wallkick
	wallKickEffects_.reset(Object2dInstancing::Create(TextureManager::Load("wallKickEffect.png"), Vector2{ 1.0f,1.0f }, 8));
	wallKickEffects_->SetSize({ float(BaseBlock::kBlockSize_),float(BaseBlock::kBlockSize_) });

	//chargejump
	jumpChargeParticles_.reset(Object2dInstancing::Create(TextureManager::Load("circle.png"), Vector2{ 1.0f,1.0f }, 8));
	jumpChargeParticles_->SetSize({ float(BaseBlock::kBlockSize_/6),float(BaseBlock::kBlockSize_/6) });

	//chargeComplete
	chargeCompleteEffect_.reset(Object2d::Create(TextureManager::Load("circle.png"), { 0,0 }, 1.0f));
	chargeCompleteEffect_->SetSize({12.0f,12.0f});

	//goldWave
	goldBlockEffect_.reset(Object2d::Create(TextureManager::Load("goldEffect.png"), { 0,0 }, 1.0f));
	goldBlockEffect_->SetSize({float(Block::kBlockSize_),float(Block::kBlockSize_) });
}

void BlockTextureManager::ClearObject() {
	for (uint32_t index=1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		objects_[index-1]->ClearUseCount();
		breakParticles_[index - 1]->ClearUseCount();
	}
	starParticles_->ClearUseCount();
	sandParticles_->ClearUseCount();
	gemParticles_->ClearUseCount();
	starParticlesUI_->ClearUseCount();

	wallKickEffects_->ClearUseCount();
	jumpChargeParticles_->ClearUseCount();
	hitEffects_->ClearUseCount();
}

void BlockTextureManager::AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize, uint32_t type, float disolveValue) {
	BaseBlock::BlockType bType = BaseBlock::BlockType(type);
	if (bType == BaseBlock::BlockType::kNone) {
		return;
	}
	Vector2 size = { float(BaseBlock::kBlockSize_), float(BaseBlock::kBlockSize_) };
	objects_[bType-1]->AppendObject(position,0,texBase,texSize, disolveValue);
}

void BlockTextureManager::AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize, uint32_t type, const Vector4& color, float disolveValue) {
	BaseBlock::BlockType bType = BaseBlock::BlockType(type);
	if (bType == BaseBlock::BlockType::kNone) {
		return;
	}
	Vector2 size = {float(BaseBlock::kBlockSize_), float(BaseBlock::kBlockSize_)};
	objects_[bType - 1]->AppendObject(position, size, 0, texBase, texSize, color, disolveValue);
}

void BlockTextureManager::DrawAll(const Camera& camera) {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		objects_[index - 1]->Draw(camera);
	}
}

void BlockTextureManager::DrawHeat(const Camera& camera) {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		if (index != BaseBlock::BlockType::kSnow) {
			objects_[index - 1]->Draw(camera);
		}
	}
}

void BlockTextureManager::DrawCold(const Camera& camera) {
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		if (index != BaseBlock::BlockType::kMagma) {
			objects_[index - 1]->Draw(camera);
		}
	}
}

void BlockTextureManager::AppendParticle(const Vector2& position, float rotate,const Vector2& uvBase, uint32_t type, const Vector4& color) {
	BaseBlock::BlockType bType = BaseBlock::BlockType(type);
	if (bType == BaseBlock::BlockType::kNone) {
		return;
	}
	breakParticles_[bType - 1]->AppendObject(position, breakParticles_[bType - 1]->GetSize(), rotate, uvBase, Vector2{32.0f,32.0f}, color, 0);
}

void BlockTextureManager::AppendStarParticle(const Vector2& position,const Vector4& color) {
	
	starParticles_->AppendObject(position, starParticles_->GetSize(), 0, Vector2{0,0}, Vector2{64.0f,64.0f}, color, 0);
}

void BlockTextureManager::AppendStarParticleUI(const Vector2& position, const Vector2& size, const Vector4& color) {

	starParticlesUI_->AppendObject(position, size, 0, Vector2{0,0}, Vector2{64.0f,64.0f}, color, 0);
}

void BlockTextureManager::AppendSandParticle(const Vector2& position, const Vector4& color) {

	sandParticles_->AppendObject(position, sandParticles_->GetSize() , 0, Vector2{ 0,0 }, Vector2{ 16.0f,16.0f }, color, 0);
}

void BlockTextureManager::AppendGemParticle(const Vector2& position,uint32_t type, const Vector4& color) {
	Vector2 tBase = {0,0};
	if (type == Block::kRedBlock) {
		tBase = { 32,0 };
	}
	else if (type == Block::kGreenBlock) {
		tBase = {64,0};
	}
	else if (type == Block::kBlueBlock) {
		tBase = { 96,0 };
	}

	gemParticles_->AppendObject(position, gemParticles_->GetSize(), 0, tBase, Vector2{32.0f,32.0f}, color, 0);
}

void BlockTextureManager::AppendWallKickEffect(const Vector2& position, uint32_t type, const Vector4& color) {
	Vector2 tBase = { 0,0 };
	Vector2 tSize = { 0,0 };
	if (type == 1) {
		tBase = { 0,0 };
		tSize = {32.0f,32.0f};
	}
	else if (type == 0) {
		tBase = { 32.0f,0 };
		tSize = { -32.0f,32.0f };
	}

	wallKickEffects_->AppendObject(position, wallKickEffects_->GetSize(), 0, tBase, tSize, color, 0);
}

//描画オブジェクト追加
void BlockTextureManager::AppendHitEffect(const Vector2& position,const Vector2& size, float rotate, const Vector4& color) {
	//Vector2 size = { float(BaseBlock::kBlockSize_) * 0.25f, float(BaseBlock::kBlockSize_) * 30.0f };
	//size.x *= RandomEngine::GetRandom(1.0f,2.0f);
	//size.y *= RandomEngine::GetRandom(0.7f, 1.5f);
	hitEffects_->AppendObject(position, size, rotate, Vector2{0,0}, Vector2{64.0f,64.0f}, color, 0);
}

void BlockTextureManager::AppendJumpChargeParticle(const Vector2& position,float rotate, uint32_t type, const Vector4& color) {
	Vector2 tBase = { 0,0 };
	Vector2 tSize = { 128,128 };

	jumpChargeParticles_->AppendObject(position, jumpChargeParticles_->GetSize(), rotate, tBase, tSize, color, 0);
}
void BlockTextureManager::DrawParticle(const Camera& camera) {
	for (std::unique_ptr<BlockBreakParticle>& data : breakParticleDatas_) {
		data->Draw();
	}
	for (uint32_t index = 1; index < BaseBlock::BlockType::kMaxBlock; index++) {
		breakParticles_[index - 1]->Draw(camera);
	}
	for (std::unique_ptr<StarParticle>& data : starParticleDatas_) {
		data->Draw();
	}
	starParticles_->Draw(camera);
	for (std::unique_ptr<SandParticle>& data : sandParticleDatas_) {
		data->Draw();
	}
	sandParticles_->Draw(camera);
	for (std::unique_ptr<GemGetParticle>& data : gemParticleDatas_) {
		data->Draw();
	}
	gemParticles_->Draw(camera);

	for (std::unique_ptr<WallKickEffect>& data : wallKickEffectDatas_) {
		data->Draw();
	}
	wallKickEffects_->Draw(camera);

	for (std::unique_ptr<JumpChargeParticle>& data : jumpChargeParticleDatas_) {
		data->Draw();
	}
	jumpChargeParticles_->Draw(camera);
	Object2d::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	GoldBlockEffectDraw(camera);
	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (std::unique_ptr<HitEffect>& data : hitEffectDatas_) {
		data->Draw();
	}
	//hitEffects_->Draw(camera);
}

void BlockTextureManager::DrawHitEffect(const Camera& camera) {
	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	hitEffects_->Draw(camera);
}

void BlockTextureManager::DrawParticleUI() {
	for (std::unique_ptr<StarParticle>& data : starParticleDatasUI_) {
		data->DrawUI();
	}
	Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	starParticlesUI_->Draw(*constantCamera_.get());
	Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
}

void BlockTextureManager::CreateParticle(const Vector2& position, uint32_t type) {
	std::unique_ptr<BlockBreakParticle> particle;
	particle.reset(new BlockBreakParticle);
	particle->Initialize(position,type);
	breakParticleDatas_.push_back(std::move(particle));
}

void BlockTextureManager::CreateParticle(const Vector2& position, const Vector2& velocity, uint32_t type) {
	std::unique_ptr<BlockBreakParticle> particle;
	particle.reset(new BlockBreakParticle);
	particle->Initialize(position,velocity, type);
	breakParticleDatas_.push_back(std::move(particle));
}

void BlockTextureManager::CreateStarParticle(const Vector2& position,int32_t type) {
	StarParticle::response--;
	if (StarParticle::response<0) {
		std::unique_ptr<StarParticle> particle;
		particle.reset(new StarParticle);
		particle->Initialize(position,type);
		starParticleDatas_.push_back(std::move(particle));
	}
}

void BlockTextureManager::CreateWallKickEffect(const Vector2& position, int32_t type) {
	std::unique_ptr<WallKickEffect> particle;
	particle.reset(new WallKickEffect);
	particle->Initialize(position, type);
	wallKickEffectDatas_.push_back(std::move(particle));
}

void BlockTextureManager::CreateJumpChargeParticle(const Vector2& position, int32_t type) {
	JumpChargeParticle::response--;
	if (JumpChargeParticle::response < 0) {
		std::unique_ptr<JumpChargeParticle> particle;
		particle.reset(new JumpChargeParticle);
		particle->Initialize(position, type);
		jumpChargeParticleDatas_.push_back(std::move(particle));
	}
}

void BlockTextureManager::CreateHitEffect(const Vector2& position, int32_t type) {
	std::unique_ptr<HitEffect> particle;
	particle.reset(new HitEffect);
	particle->Initialize(position,type);
	hitEffectDatas_.push_back(std::move(particle));
}

void BlockTextureManager::CreateStarParticleUI(const Vector2& position, int32_t type) {
		std::unique_ptr<StarParticle> particle;
		particle.reset(new StarParticle);
		particle->Initialize(position, type);
		starParticleDatasUI_.push_back(std::move(particle));
}

void BlockTextureManager::CreateSandParticle(const Vector2& position, int32_t type) {
	SandParticle::response--;
	if (SandParticle::response < 0) {
		std::unique_ptr<SandParticle> particle;
		particle.reset(new SandParticle);
		particle->Initialize(position, type);
		sandParticleDatas_.push_back(std::move(particle));
	}
}

void BlockTextureManager::CreateGemParticle(const Vector2& position, int32_t type) {
	std::unique_ptr<GemGetParticle> particle;
	particle.reset(new GemGetParticle);
	particle->Initialize(position, type);
	gemParticleDatas_.push_back(std::move(particle));
	
}

void BlockTextureManager::UpdateParticle(const Camera& camera) {
	//block
	breakParticleDatas_.remove_if([](std::unique_ptr<BlockBreakParticle>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<BlockBreakParticle> &data : breakParticleDatas_) {
		data->Update();
	}
	//star
	starParticleDatas_.remove_if([](std::unique_ptr<StarParticle>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<StarParticle>& data : starParticleDatas_) {
		data->Update();
	}
	//sand
	sandParticleDatas_.remove_if([](std::unique_ptr<SandParticle>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<SandParticle>& data : sandParticleDatas_) {
		data->Update();
	}
	//gem
	gemParticleDatas_.remove_if([](std::unique_ptr<GemGetParticle>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<GemGetParticle>& data : gemParticleDatas_) {
		data->Update(camera);
	}
	//starUI
	starParticleDatasUI_.remove_if([](std::unique_ptr<StarParticle>& bullet) {
	if (!bullet->GetIsAlive()) {
		return true;
	}
	return false;
		});
	for (std::unique_ptr<StarParticle>& data : starParticleDatasUI_) {
		data->Update();
	}

	//wallKick
	wallKickEffectDatas_.remove_if([](std::unique_ptr<WallKickEffect>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<WallKickEffect>& data : wallKickEffectDatas_) {
		data->Update();
	}

	//hitEffect
	hitEffectDatas_.remove_if([](std::unique_ptr<HitEffect>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<HitEffect>& data : hitEffectDatas_) {
		data->Update();
	}
}

void BlockTextureManager::UpdateJumpChargeParticle(const Vector2& position) {
	//JumpChargeParticle
	jumpChargeParticleDatas_.remove_if([](std::unique_ptr<JumpChargeParticle>& bullet) {
		if (!bullet->GetIsAlive()) {
			return true;
		}
		return false;
		});
	for (std::unique_ptr<JumpChargeParticle>& data : jumpChargeParticleDatas_) {
		data->Update(position);
	}
}

void BlockTextureManager::GoldBlockEffectUpdate() {

}
void BlockTextureManager::GoldBlockEffectDraw(const Camera& camera) {
	if (!isDrawGoldBlockEffect_) {
		return;
	}
	goldBlockEffect_->Draw(camera);
	isDrawGoldBlockEffect_ = false;
}

void BlockTextureManager::UpdateChargeCompleteEffect(const Vector2& position) {
	chargeCompleteTime++;
	if (chargeCompleteTime>=chargeCompleteEnd) {
		chargeCompleteTime = 0;
	}
	chargeCompleteEffect_->position_ = position;
	chargeCompleteEffect_->SetScale(1.1f+( float(chargeCompleteTime) / float(chargeCompleteEnd)) * 8.2f);
	float c = (float(chargeCompleteEnd - chargeCompleteTime) / float(chargeCompleteEnd)) * 0.8f;
	chargeCompleteEffect_->SetColor({1000.0f,1000.0f,1000.0f,c});
	isDrawChargeCompleteEffect_ = true;
}

void BlockTextureManager::DrawChargeCompleteEffect(const Camera& camera) {
	if (!isDrawChargeCompleteEffect_) {
		chargeCompleteTime = 0;
		return;
	}
	chargeCompleteEffect_->Draw(camera);
	isDrawChargeCompleteEffect_ = false;
}

void BlockTextureManager::UpdateGoldWaveEffectEffect(const Vector2& position) {
	static uint32_t count;
	count++;
	if (count >= goldWaveEffectEnd) {
		count = 0;
	}
	goldBlockEffect_->position_ = position;
	goldBlockEffect_->SetScale(1.0f + (float(count) / float(goldWaveEffectEnd)));
	float c = (float(goldWaveEffectEnd - count) / float(goldWaveEffectEnd)) * 0.8f;
	goldBlockEffect_->SetColor({ 1.0f,1.0f,1.0f,c });
	isDrawGoldBlockEffect_ = true;
}