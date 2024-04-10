#pragma once
#include "CameraFrozen.h"
#include <memory>
class CameraFrozenManager {
public:
	static CameraFrozenManager* GetInstance() {
		static CameraFrozenManager instance;
		return &instance;
	}
	void Init();
	void Update();
	void Draw(ID3D12GraphicsCommandList* cmdList);
	/// <summary>
	///	内部テクスチャ書き込み
	/// </summary>
	/// <param name="cmdList"></param>
	void DrawInternal(ID3D12GraphicsCommandList* cmdList);
	//アニメーション開始
	void Start() {
		isActive_ = true;
		radius_ = radiusMax_;
		isClearRender_ = true;
		cameraFrozen_->SetMode(0);
	};
	//アニメーションリセット(エフェクトクリア)
	void Reset() {
		//isClearRender_ = true;
		isActive_ = true;
		radius_ = radiusMax_;
		cameraFrozen_->SetMode(1);
	}

	PostEffect* GetEffectTexture() { return cameraFrozen_.get(); };

private:
	CameraFrozenManager() {
		Init();
	}
	bool isClearRender_;
	bool isActive_;
	float radius_;
	float border_;
	float radiusMax_;
	float radiusMin_;
	float seed_;
	std::unique_ptr<CameraFrozen> cameraFrozen_;
};