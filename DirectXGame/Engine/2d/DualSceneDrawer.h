#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include "Vec2.h"
#include "Vec4.h"
#include "Matrix44.h"
#include <string>
#include <array>

class DualSceneDrawer {
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public: //メンバ関数

	DualSceneDrawer();

	void Init();
	
	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	/// <param name="prevSceneHandle">前シーンのハンドル</param>
	/// <param name="nextSceneHandle">次シーンのハンドル</param>
	/// <param name="weightHandle">weightMapのハンドル</param>
	void Draw(ID3D12GraphicsCommandList* cmdList, D3D12_GPU_DESCRIPTOR_HANDLE prevSceneHandle, D3D12_GPU_DESCRIPTOR_HANDLE nextSceneHandle, D3D12_GPU_DESCRIPTOR_HANDLE weightHandle);

	void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

	void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// SRV指定用ハンドル取得
	/// </summary>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSrvHandleGPU() { return textureSrvHandleGPU_; };
private:

	void CreateGraphicsPipelineState();
	
	ComPtr<ID3D12Resource> CreateBufferResource(ID3D12Device* device, size_t sizeInBytes);

	ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandleer);

	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, UINT descriptorSize, UINT index);

	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(ComPtr<ID3D12DescriptorHeap> descriptorHeap, UINT descriptorSize, UINT index);

private: //メンバ変数

	static const float clearColor_[4];

	ComPtr<ID3D12PipelineState> piplineState_;
	ComPtr<ID3D12RootSignature> rootSignature_;

	ComPtr<ID3D12Resource> texBuff_;
	ComPtr<ID3D12Resource> depthBuff_;

	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU_{};
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU_{};

	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandleCPU_{};
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandleCPU_{};

};

