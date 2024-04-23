#pragma once
#include <vector>
#include <memory>
#include "Object2dInstancing.h"
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
	uint32_t GetBlockTexture(uint32_t type) { return blockTextures_[type]; }

	/// <summary>
	/// 全ブロック画像をロード
	/// </summary>
	void LoadAllBlockTexture();

	//描画用オブジェクト配列をクリアする
	void ClearObject();

	//描画オブジェクト追加
	void AppendObject(const Vector2& position, const Vector2& texBase, const Vector2& texSize,uint32_t type);

	//全オブジェクト描画
	void DrawAll(const Camera& camera);

private:

	std::vector<uint32_t> blockTextures_;

	std::vector<std::unique_ptr<Object2dInstancing>> objects_;

private:

	BlockTextureManager();
	~BlockTextureManager() = default;
	BlockTextureManager(const BlockTextureManager&) = delete;
	const BlockTextureManager& operator=(const BlockTextureManager&) = delete;

};


