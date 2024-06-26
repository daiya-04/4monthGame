#pragma once
#include "Vec3.h"
#include "Vec2.h"
#include "Matrix44.h"
#include <d3d12.h>
#include <wrl.h>
#include <numbers>

struct CBufferDataCamera {
	Matrix4x4 matView;
	Matrix4x4 matProjection;
	Vector3 cameraPos;
};

class Camera {
public:

	Vector3 translation_ = { 0.0f, 0.0f, 0.0f };
	Vector3 rotation_ = {};

	Vector2 center_ = { 1280.0f / 2.0f,720.0f / 2.0f };

private:

	float fovAngleY = 45.0f * std::numbers::pi_v<float> / 180.0f;
	float aspectRatio = (float)16 / (float)9;
	float nearZ = 0.1f;
	float farZ = 1000.0f;

	Vector2 drawingRange_ = { 1280.0f,720.0f };

	Matrix4x4 matView_ = MakeIdentity44();
	Matrix4x4 matProjection_ = MakeIdentity44();

	Microsoft::WRL::ComPtr<ID3D12Resource> cBuffer_;
	CBufferDataCamera* cMap_ = nullptr;

public:

	void Init();

	void UpdateMatrix();

	void UpdateViewMatrix();

	void UpdateProjectionMatrix();

	void ChangeDrawingRange(const Vector2& drawingRange);

	D3D12_GPU_VIRTUAL_ADDRESS GetGPUVirtualAddress() const { return cBuffer_->GetGPUVirtualAddress(); }

	Matrix4x4 GetBillBoadMatrix() const {
		Matrix4x4 billboardMat = matView_;
		billboardMat = billboardMat.Inverse();
		billboardMat.m[3][0] = 0.0f;
		billboardMat.m[3][1] = 0.0f;
		billboardMat.m[3][2] = 0.0f;

		return billboardMat;
	}

	Vector2 GetDrawingRange() const { return drawingRange_; }

	//viewMatrixを取得
	Matrix4x4 GetMatView() { return matView_; };

	//projectionMatrixを取得
	Matrix4x4 GetMatProjection() { return matProjection_; };

private:

	void CreateCBuffer();
	void Map();

};

