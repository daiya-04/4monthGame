#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include "Vec2.h"
#include "Vec4.h"
#include "Matrix44.h"
#include <string>
#include <array>
#include "Camera.h"
#include "TextureManager.h"

class Object2d {
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	enum RootParameter {
		kMaterial,
		kWorldTransform,
		kCamera,
		kTexture,

		kParamNum,
	};

public:

	struct VertexData {
		Vector4 pos_;
		Vector2 uv_;
	};

	struct MaterialData {
		Vector4 color_;
	};

private: //静的メンバ変数

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static ComPtr<ID3D12RootSignature> rootSignature_;
	static ComPtr<ID3D12PipelineState> graphicsPipelineState_;

public: //静的メンバ関数

	static void StaticInitialize(ID3D12Device* device, int windowWidth, int windowHeight);

	static Object2d* Create(uint32_t textureHandle, Vector2 postion, float scale = 1.0f);

	static void preDraw(ID3D12GraphicsCommandList* commandList);

	static void postDraw();

private:

	static ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandleer);

private: //メンバ変数

	ComPtr<ID3D12Resource> matrialResource_;
	ComPtr<ID3D12Resource> worldMatResource_;
	Matrix4x4* worldMatMap_ = nullptr;
	ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	D3D12_RESOURCE_DESC resourceDesc_;

	//サイズ
	Vector2 size_ = { 100.0f,100.0f };
	//倍率
	Vector2 scale_ = { 1.0f,1.0f };
	//アンカーポイント
	Vector2 anchorpoint_ = { 0.5f, 0.5f };
	//色(RGBA)
	Vector4 color_{ 1.0f,1.0f,1.0f,1.0f };
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	//テクスチャ左上座標
	Vector2 texBase_{};
	//テクスチャサイズ
	Vector2 texSize_ = { 100.0f,100.0f };

public:
	//座標
	Vector2 position_{};
	
	//回転
	float rotate_{};
	

public:

	Object2d(uint32_t textureHandle, Vector2 position, float scale = 1.0f, Vector4 color = { 1.0f,1.0f,1.0f,1.0f });

	void Init();

	void Draw(const Camera& camera);

	void SetSize(const Vector2& size);

	void SetScale(float scale);
	void SetScale(const Vector2& scale);

	void SetAnchorpoint(const Vector2& anchorpoint);

	void SetColor(const Vector4& color);

	void SetTextureHandle(uint32_t textureHandle) { 
		textureHandle_ = textureHandle;
		resourceDesc_ = TextureManager::GetInstance()->GetResourceDesc(textureHandle_);
	}

	void SetTextureArea(const Vector2& texBase, const Vector2& texSize);

	uint32_t GetTextureHandle() {
		return textureHandle_;
	}

private:

	void TransferVertex();

	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

};

