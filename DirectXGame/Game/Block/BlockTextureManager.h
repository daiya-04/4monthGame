#pragma once
#include <vector>

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

private:

	std::vector<uint32_t> blockTextures_;

private:

	BlockTextureManager() = default;
	~BlockTextureManager() = default;
	BlockTextureManager(const BlockTextureManager&) = delete;
	const BlockTextureManager& operator=(const BlockTextureManager&) = delete;

};


