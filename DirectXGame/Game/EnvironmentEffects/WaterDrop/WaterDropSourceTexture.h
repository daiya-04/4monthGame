#pragma once
#include "PostEffect.h"

class WaterDropSourceTexture : public PostEffect
{
public:
	void Init() override;
	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	WaterDropSourceTexture() = default;
	~WaterDropSourceTexture() = default;

	
private:
	
	static const float clearColorWaterDrop_[4];//水滴の重ねようの専用クリアカラー
};
