#include "WaterDropSourceTexture.h"
#include <d3dx12.h>
#include <DirectXTex.h>
#include <format>
#include <cassert>
#include <d3dcompiler.h>
#include "Log.h"
#include "WinApp.h"
#include "DirectXCommon.h"
#include "TextureManager.h"

#pragma comment(lib,"d3dcompiler.lib")

using namespace Microsoft::WRL;

const float WaterDropSourceTexture::clearColorWaterDrop_[4] = { 0.0f,0.0f,0.0f,0.0f };

void WaterDropSourceTexture::Init() {
	HRESULT hr;

	CreateGraphicsPipelineState(L"Resources/shaders/PostEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

	//テクスチャリソースの設定
	D3D12_RESOURCE_DESC texDesc{};
	texDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	texDesc.Width = WinApp::kClientWidth;
	texDesc.Height = WinApp::kClientHeight;
	texDesc.DepthOrArraySize = 1;
	texDesc.MipLevels = 1;
	texDesc.SampleDesc.Count = 1;
	texDesc.SampleDesc.Quality = 0;
	texDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	texDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	//利用するヒープの設定
	D3D12_HEAP_PROPERTIES srvHeapProperties{};
	//heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //VRAM上に作成
	srvHeapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	srvHeapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	srvHeapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	D3D12_CLEAR_VALUE clearColorValue{};
	clearColorValue.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	memcpy(clearColorValue.Color, clearColorWaterDrop_, sizeof(float) * 4);

	//テクスチャバッファの生成
	hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&srvHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&texDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		&clearColorValue,
		IID_PPV_ARGS(&texBuff_)
	);
	assert(SUCCEEDED(hr));

	textureSrvHandleCPU_ = GetCPUDescriptorHandle(DirectXCommon::GetInstance()->GetSrvHeap(), DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), DirectXCommon::GetInstance()->GetSrvHeapCount());
	textureSrvHandleGPU_ = GetGPUDescriptorHandle(DirectXCommon::GetInstance()->GetSrvHeap(), DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV), DirectXCommon::GetInstance()->GetSrvHeapCount());

	DirectXCommon::GetInstance()->IncrementSrvHeapCount();

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = 1;

	DirectXCommon::GetInstance()->GetDevice()->CreateShaderResourceView(texBuff_.Get(), &srvDesc, textureSrvHandleCPU_);

	//RTVの設定
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;

	rtvHandleCPU_ = GetCPUDescriptorHandle(DirectXCommon::GetInstance()->GetRtvHeap(), DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV), DirectXCommon::GetInstance()->GetRtvHeapCount());
	DirectXCommon::GetInstance()->IncrementRtvHeapCount();

	DirectXCommon::GetInstance()->GetDevice()->CreateRenderTargetView(texBuff_.Get(), &rtvDesc, rtvHandleCPU_);

	dsvHandleCPU_ = GetCPUDescriptorHandle(DirectXCommon::GetInstance()->GetDsvHeap(), DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV), 0);

}

void WaterDropSourceTexture::PreDrawScene(ID3D12GraphicsCommandList* cmdList) {

	//リソースバリアの変更(シェーダーリソース描画可能)
	D3D12_RESOURCE_BARRIER barrier{};
	//今回のバリアはTransition
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//Noneにしておく
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//バリアを張る対象のリリース。
	barrier.Transition.pResource = texBuff_.Get();
	//遷移前（現在）のResourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//遷移後のResourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	cmdList->ResourceBarrier(1, &barrier);

	//レンダーターゲットをセット
	cmdList->OMSetRenderTargets(1, &rtvHandleCPU_, false, &dsvHandleCPU_);

	//ビューポート
	D3D12_VIEWPORT viewport{};
	//クライアント領域のサイズと一緒にして画面全体に表示
	viewport.Width = WinApp::kClientWidth;
	viewport.Height = WinApp::kClientHeight;
	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	cmdList->RSSetViewports(1, &viewport);  //Viewportを設定

	//シザー矩形
	D3D12_RECT scissorRect{};
	//基本的にビューポートと同じ矩形が構成されるようにする
	scissorRect.left = 0;
	scissorRect.right = WinApp::kClientWidth;
	scissorRect.top = 0;
	scissorRect.bottom = WinApp::kClientHeight;

	cmdList->RSSetScissorRects(1, &scissorRect);  //Scissorを設定

	//全画面クリア
	cmdList->ClearRenderTargetView(rtvHandleCPU_, clearColorWaterDrop_, 0, nullptr);
	//深度バッファのクリア
	cmdList->ClearDepthStencilView(dsvHandleCPU_, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

	//描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { DirectXCommon::GetInstance()->GetSrvHeap() };
	cmdList->SetDescriptorHeaps(_countof(descriptorHeaps), descriptorHeaps);



}