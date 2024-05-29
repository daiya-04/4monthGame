#pragma once
#include <vector>
#include <memory>
#include "Object2dInstancing.h"
#include <list>
#include "Vec4.h"
class BlockBreakParticle;
class StarParticle;
class SandParticle;

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
	void AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize,uint32_t type);

	//描画オブジェクト追加
	void AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize, uint32_t type,const Vector4& color);

	//全オブジェクト描画
	void DrawAll(const Camera& camera);

	//灼熱描画
	void DrawHeat(const Camera& camera);

	//極寒描画
	void DrawCold(const Camera& camera);

	//描画オブジェクト追加
	void AppendParticle(const Vector2& position, uint32_t type);

	//描画オブジェクト追加
	void AppendStarParticle(const Vector2& position,const Vector4& color);

	//描画オブジェクト追加
	void AppendSandParticle(const Vector2& position, const Vector4& color);

	//全オブジェクト描画
	void DrawParticle(const Camera& camera);

	void CreateParticle(const Vector2& position, uint32_t type);
	void CreateParticle(const Vector2& position, const Vector2& velocity, uint32_t type);

	void CreateStarParticle(const Vector2& position, int32_t type);
	void CreateSandParticle(const Vector2& position, int32_t type);

	void UpdateParticle();

private:

	std::vector<uint32_t> blockTextures_;

	std::vector<std::unique_ptr<Object2dInstancing>> objects_;

	std::vector<std::unique_ptr<Object2dInstancing>> breakParticles_;

	std::list<std::unique_ptr<BlockBreakParticle>> breakParticleDatas_;

	std::list<std::unique_ptr<StarParticle>> starParticleDatas_;
	std::unique_ptr<Object2dInstancing> starParticles_;

	std::list<std::unique_ptr<SandParticle>> sandParticleDatas_;
	std::unique_ptr<Object2dInstancing> sandParticles_;

private:

	BlockTextureManager();
	~BlockTextureManager() = default;
	BlockTextureManager(const BlockTextureManager&) = delete;
	const BlockTextureManager& operator=(const BlockTextureManager&) = delete;

};


