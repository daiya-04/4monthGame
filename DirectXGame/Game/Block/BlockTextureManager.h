#pragma once
#include <vector>
#include <memory>
#include "Object2dInstancing.h"
#include "Object2d.h"
#include "Camera.h"
#include <list>
#include "Vec4.h"


class BlockBreakParticle;
class StarParticle;
class SandParticle;
class GemGetParticle;
class WallKickEffect;
class JumpChargeParticle;
class HitEffect;
/// <summary>
/// ブロックに使うテクスチャ一括管理
/// </summary>
class BlockTextureManager
{
public:
	
	//インスタンス取得
	static BlockTextureManager* GetInstance();

	/// <summary>
	/// ブロックの使用する画像取得
	/// </summary>
	/// <param name="type">ブロックの種類</param>
	/// <returns></returns>
	/*uint32_t GetBlockTexture(uint32_t type) { return blockTextures_[type]; }*/

	/// <summary>
	/// 全ブロック画像をロード
	/// </summary>
	void LoadAllBlockTexture();

	//描画用オブジェクト配列をクリアする
	void ClearObject();

	//描画オブジェクト追加
	void AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize,uint32_t type, float disolveValue);

	//描画オブジェクト追加
	void AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize, uint32_t type,const Vector4& color,float disolveValue);

	//全オブジェクト描画
	void DrawAll(const Camera& camera);

	//灼熱描画
	void DrawHeat(const Camera& camera);

	//極寒描画
	void DrawCold(const Camera& camera);

	//描画オブジェクト追加
	void AppendParticle(const Vector2& position,float rotate,const Vector2& uvBase, uint32_t type);

	//描画オブジェクト追加
	void AppendStarParticle(const Vector2& position,const Vector4& color);

	//描画オブジェクト追加
	void AppendSandParticle(const Vector2& position, const Vector4& color);

	//描画オブジェクト追加
	void AppendGemParticle(const Vector2& position, uint32_t type, const Vector4& color);

	//描画オブジェクト追加
	void AppendWallKickEffect(const Vector2& position, uint32_t type, const Vector4& color);

	//描画オブジェクト追加
	void AppendJumpChargeParticle(const Vector2& position,float rotate, uint32_t type, const Vector4& color);

	//描画オブジェクト追加
	void AppendHitEffect(const Vector2& position, float rotate, const Vector4& color);

	//描画オブジェクト追加
	void AppendStarParticleUI(const Vector2& position, const Vector4& color);

	//全オブジェクト描画
	void DrawParticle(const Camera& camera);

	void DrawParticleUI();

	void CreateParticle(const Vector2& position, uint32_t type);
	void CreateParticle(const Vector2& position, const Vector2& velocity, uint32_t type);

	void CreateStarParticle(const Vector2& position, int32_t type);
	void CreateSandParticle(const Vector2& position, int32_t type);
	void CreateGemParticle(const Vector2& position, int32_t type);
	void CreateWallKickEffect(const Vector2& position, int32_t type);
	void CreateJumpChargeParticle(const Vector2& position, int32_t type);
	void CreateStarParticleUI(const Vector2& position,int32_t type);
	void CreateHitEffect(const Vector2& position, int32_t type);
	void UpdateParticle(const Camera& camera);

	void UpdateJumpChargeParticle(const Vector2& position);

	void UpdateGoldWaveEffectEffect(const Vector2& position);

	void UpdateChargeCompleteEffect(const Vector2& position);
	void DrawChargeCompleteEffect(const Camera& camera);
	void SetPlayerTextureHandle(uint32_t handle){
		chargeCompleteEffect_->SetTextureHandle(handle);
	}
	void SetPlayerTextureArea(const Vector2& base, const Vector2& size) {
		chargeCompleteEffect_->SetTextureArea(base,size);
	};
	void SetPlayerTextureSize(const Vector2& size) {
		chargeCompleteEffect_->SetSize(size);
	}

	//黄金ブロックのエフェクト
	void GoldBlockEffectUpdate();
	void GoldBlockEffectDraw(const Camera& camera);
	//void SetGoldBlockEffectPosition(const Vector2& pos) { goldBlockEffect_->position_ = pos; };
	void SetIsResetGoldBlockEffect(bool is) { isResetGoldBlockEffect_ = is; };
	//指定の番号にテクスチャ変更
	void ChangeTexture(int32_t type, int32_t index) {
		objects_[type - 1]->SetTextureHandle(blockTextures_[index]);
		breakParticles_[type - 1]->SetTextureHandle(blockTextures_[index]);
	}


private:

	std::vector<uint32_t> blockTextures_;

	std::vector<std::unique_ptr<Object2dInstancing>> objects_;

	std::vector<std::unique_ptr<Object2dInstancing>> breakParticles_;

	std::list<std::unique_ptr<BlockBreakParticle>> breakParticleDatas_;

	std::list<std::unique_ptr<StarParticle>> starParticleDatas_;
	std::unique_ptr<Object2dInstancing> starParticles_;

	std::list<std::unique_ptr<SandParticle>> sandParticleDatas_;
	std::unique_ptr<Object2dInstancing> sandParticles_;

	std::list<std::unique_ptr<GemGetParticle>> gemParticleDatas_;
	std::unique_ptr<Object2dInstancing> gemParticles_;

	std::list<std::unique_ptr<WallKickEffect>> wallKickEffectDatas_;
	std::unique_ptr<Object2dInstancing> wallKickEffects_;

	std::list<std::unique_ptr<JumpChargeParticle>> jumpChargeParticleDatas_;
	std::unique_ptr<Object2dInstancing> jumpChargeParticles_;

	std::list<std::unique_ptr<HitEffect>> hitEffectDatas_;
	std::unique_ptr<Object2dInstancing> hitEffects_;

private:

	BlockTextureManager();
	~BlockTextureManager() = default;
	BlockTextureManager(const BlockTextureManager&) = delete;
	const BlockTextureManager& operator=(const BlockTextureManager&) = delete;

	std::unique_ptr<Camera> constantCamera_;//spriteをinstancing対応させるまでの仮カメラ
	std::list<std::unique_ptr<StarParticle>> starParticleDatasUI_;
	std::unique_ptr<Object2dInstancing> starParticlesUI_;

	//黄金ブロックのエフェクト
	std::unique_ptr<Object2d> goldBlockEffect_;
	bool isDrawGoldBlockEffect_=false;
	bool isResetGoldBlockEffect_;
	static const uint32_t goldWaveEffectEnd = 30;

	//タメ完了エフェクト
	bool isDrawChargeCompleteEffect_=false;
	uint32_t chargeCompleteTime=0;
	static const uint32_t chargeCompleteEnd=12;
	std::unique_ptr<Object2d> chargeCompleteEffect_;
};


