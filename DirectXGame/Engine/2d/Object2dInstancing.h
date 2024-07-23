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
#include <vector>

class Object2dInstancing {
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	enum RootParameter {
		kMaterial,
		kForVS,
		kCamera,
		kTexture,
		kMaskTexture,
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

	//VertexShaderに送るパラメータ
	struct InstancingForVSData {
		Matrix4x4 worldMat_;
		Vector2 size_;
		Vector2 texcoord_[4];
		Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
		float disolveValue_ = 0;
	};

	//CPU内で処理に使うデータ
	struct InstancingCPUData {
		//座標
		Vector2 position_{};

		//サイズ
		Vector2 size_{1.0f,1.0f};

		//テクスチャ左上座標
		Vector2 texBase_{0.0f,0.0f};
		//テクスチャサイズ
		Vector2 texSize_ = { 100.0f,100.0f };

		Vector4 color_ = {1.0f,1.0f,1.0f,1.0f};
		float rotate_=0;
		float disolveValue_ = 0;
	};

private: //静的メンバ変数

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static ComPtr<ID3D12RootSignature> rootSignature_;
	static ComPtr<ID3D12PipelineState> graphicsPipelineState_;

public: //静的メンバ関数

	static void StaticInitialize(ID3D12Device* device, int windowWidth, int windowHeight);

	static Object2dInstancing* Create(uint32_t textureHandle, Vector2 postion,uint32_t instanceMax, float scale = 1.0f);

	static void preDraw(ID3D12GraphicsCommandList* commandList);

	static void postDraw();

private:

	static ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandleer);

private: //メンバ変数

	ComPtr<ID3D12Resource> matrialResource_;
	//ComPtr<ID3D12Resource> worldMatResource_;
	//Matrix4x4* worldMatMap_ = nullptr;
	ComPtr<ID3D12Resource> instancingForVSResource_;
	InstancingForVSData* instancingForVSMap_ = nullptr;
	uint32_t instanceCount_;
	uint32_t instanceMax_;
	ComPtr<ID3D12Resource> vertexResource_;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
	D3D12_RESOURCE_DESC resourceDesc_;

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU_{};

	//サイズ
	Vector2 size_ = { 100.0f,100.0f };
	//アンカーポイント
	Vector2 anchorpoint_ = { 0.5f, 0.5f };
	//色(RGBA)
	Vector4 color_{ 1.0f,1.0f,1.0f,1.0f };
	//テクスチャハンドル
	uint32_t textureHandle_ = 0;
	uint32_t maskTextureHandle_ = 0;
	//テクスチャ左上座標
	//Vector2 texBase_{};
	//テクスチャサイズ
	//Vector2 texSize_ = { 100.0f,100.0f };

	std::vector<InstancingCPUData> instancingCPUData_;

public:
	//座標
	//Vector2 position_{};

	//回転
	float rotate_{};



public:

	Object2dInstancing(uint32_t textureHandle,float scale = 1.0f, Vector4 color = { 1.0f,1.0f,1.0f,1.0f });

	void Init(uint32_t instanceMax);

	void Draw(const Camera& camera);

	void SetSize(const Vector2& size);

	void SetScale(float scale);
	void SetScale(const Vector2& scale);

	void SetAnchorpoint(const Vector2& anchorpoint);

	void SetColor(const Vector4& color);

	void SetTextureHandle(uint32_t textureHandle);

	void SetMaskTextureHandle(uint32_t textureHandle) {
		maskTextureHandle_ = textureHandle;
	}

	uint32_t GetTextureHandle() { return textureHandle_; };

	Vector2 GetSize() { return size_; };

	void SetTextureArea(const Vector2& texBase, const Vector2& texSize);

	//使用数をクリア(0に)する
	void ClearUseCount() { 
		instanceCount_ = 0; 
		instancingCPUData_.clear();
	};

	//描画オブジェクトを追加する
	void AppendObject(const Vector2& position,float rotate, const Vector2& texBase, const Vector2& texSize,float disolveValue) {
		if (instanceCount_ >= instanceMax_) {
			return;
		}
		InstancingCPUData data;
		data.position_ = position;
		data.texBase_ = texBase + Vector2({ 0.0f,0.5f });
		data.texSize_ = texSize - Vector2({ 1.0f,1.0f });
		data.color_ = {1.0f,1.0f,1.0f,1.0f};
		data.rotate_ = rotate;
		data.disolveValue_ = disolveValue;
		instancingCPUData_.push_back(data);
		instanceCount_++;
	};

	//描画オブジェクトを追加する
	void AppendObject(const Vector2& position,float rotate, const Vector2& texBase, const Vector2& texSize,const Vector4& color,float disolveValue) {
		if (instanceCount_ >= instanceMax_) {
			return;
		}
		InstancingCPUData data;
		data.position_ = position;
		data.texBase_ = texBase + Vector2({ 0.0f,0.5f });
		data.texSize_ = texSize - Vector2({ 1.0f,1.0f });
		data.color_ = color;
		data.rotate_ = rotate;
		data.disolveValue_ = disolveValue;
		instancingCPUData_.push_back(data);
		instanceCount_++;
	};
	//描画オブジェクトを追加する
	void AppendObject(const Vector2& position, const Vector2& size, float rotate, const Vector2& texBase, const Vector2& texSize, const Vector4& color, float disolveValue) {
		if (instanceCount_ >= instanceMax_) {
			return;
		}
		InstancingCPUData data;
		data.position_ = position;
		data.size_ = size;
		data.texBase_ = texBase + Vector2({ 0.0f,0.5f });
		data.texSize_ = texSize - Vector2({ 1.0f,1.0f });
		data.color_ = color;
		data.rotate_ = rotate;
		data.disolveValue_ = disolveValue;
		instancingCPUData_.push_back(data);
		instanceCount_++;
	};

private:

	void TransferVertex();

	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, UINT descriptorSize, UINT index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, UINT descriptorSize, UINT index);

};

