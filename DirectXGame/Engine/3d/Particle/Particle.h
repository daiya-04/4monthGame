#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include <string>
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "Matrix44.h"
#include <vector>
#include <list>
#include <random>
#include "WorldTransform.h"
#include "Camera.h"
#include "Log.h"
#include "Hit.h"



class Particle{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
private:

	enum class RootParameter {
		kMaterial,
		kParticleGPU,
		kCamera,
		kTexture,

		kParamNum,
	};

	struct VertexData {
		Vector4 pos_;
		//Vector2 uv_;
	};

	struct Material {
		Vector4 color_;
	};

	struct ParticleGPU {
		Matrix4x4 matWorld;
		//Vector4 color;
	};

public:

	struct ParticleData {
		Vector2 pos_{};
		Vector2 scale_ = { 100.0f, 100.0f };
		float rotated_ = 0.0f;
		Vector2 velocity_{};
		Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };
		float lifeTime_ = 0; //生存時間
		float currentTime_ = 0; //生成されてから今の時間
	};

	struct Emitter {
		Vector2 translate_;
		uint32_t count_; //一度に生成する数
		float frequency_; //発生間隔
		float frequencyTime_; //発生してからの今の時間
	};

	struct AccelerationField {
		Vector2 acceleration_; //効果(加速度)
		AABB area_; //効果範囲
	};

private:

	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static ComPtr<ID3D12RootSignature> rootSignature_;
	static ComPtr<ID3D12PipelineState> graphicsPipelineState_;

public:

	//静的初期化
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* commandList);
	//モデルの生成
	static Particle* Create(uint32_t textureHandle,uint32_t particleNum);
	//描画前処理
	static void preDraw();
	//描画後処理
	static void postDraw();

	static ParticleData MakeNewParticle(std::mt19937& randomEngine, const Vector2& translate);

	static std::list<ParticleData> Emit(const Particle::Emitter& emitter, std::mt19937& randomEngine);

private:

	//シェーダのコンパイル
	static ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandleer);
	//リソースの生成
	static ComPtr<ID3D12Resource> CreateBufferResource(ComPtr<ID3D12Device> device, size_t sizeInBytes);

private:

	D3D12_VERTEX_BUFFER_VIEW vertexBufferView_{};
	ComPtr<ID3D12Resource> vertexResource_;
	ComPtr<ID3D12Resource> materialResource_;
	ComPtr<ID3D12Resource> wvpResource_;
	/*ComPtr<ID3D12Resource> indexResource_;
	D3D12_INDEX_BUFFER_VIEW indexBufferView_{};*/
	ComPtr<ID3D12Resource> instancingResource_;
	D3D12_CPU_DESCRIPTOR_HANDLE particleSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE particleSrvHandleGPU_{};


	int32_t uvHandle_ = 0;

public:

	

	uint32_t particleMaxNum_ = 0;
	uint32_t particleNum_ = 0;
	Vector4 color_ = { 1.0f,1.0f,1.0f,1.0f };

public:

	void Initialize(uint32_t textureHandle, uint32_t particleNum);

	void Draw(std::list<ParticleData>& particleData,const Camera& camera);

private:

	void CreateMesh();

	void TransferVertex();

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, UINT descriptorSize, UINT index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, UINT descriptorSize, UINT index);

};

