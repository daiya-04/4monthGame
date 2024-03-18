#pragma once
#include "CameraFrozen.h"
#include <memory>
class CameraFrozenManager {
public:
	void Init();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	///	内部テクスチャ書き込み
	/// </summary>
	/// <param name="cmdList"></param>
	void DrawInternal(ID3D12GraphicsCommandList* cmdList);
private:
	bool isClearRender_;
	bool isActive_;
	float radius_;
	float border_;
	float radiusMax_;
	float radiusMin_;
	float seed_;
	std::unique_ptr<CameraFrozen> cameraFrozen_;
};