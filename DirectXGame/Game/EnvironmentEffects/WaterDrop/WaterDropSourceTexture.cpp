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

const float WaterDropSourceTexture::clearColorWaterDrop_[4] = { 0.5f,0.5f,0.0f,0.5f };

void WaterDropSourceTexture::Init() {
	HRESULT hr;

	CreateGraphicsPipelineState(L"Resources/shaders/NoneEffect.VS.hlsl", L"Resources/shaders/NoneEffect.PS.hlsl");

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


	//画素数
	const UINT pixelCount = UINT(WinApp::kClientWidth * WinApp::kClientHeight);
	//画像1行分のデータサイズ
	const UINT rowPitch = sizeof(UINT) * WinApp::kClientWidth;
	//画像全体のデータサイズ
	const UINT depthPitch = rowPitch * WinApp::kClientHeight;
	//画像イメージ
	UINT* img = new UINT[pixelCount];
	for (int i = 0; i < pixelCount; i++) { img[i] = 0xffffffff; }

	hr = texBuff_->WriteToSubresource(0, nullptr, img, rowPitch, depthPitch);
	assert(SUCCEEDED(hr));
	delete[] img;

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

	//深度バッファリソースの設定
	D3D12_RESOURCE_DESC depResourceDesc{};
	depResourceDesc.Width = WinApp::kClientWidth; //Textureの幅
	depResourceDesc.Height = WinApp::kClientHeight; //Textureの高さ
	depResourceDesc.MipLevels = 1; //mipmapの数
	depResourceDesc.DepthOrArraySize = 1; //奥行き or 配列Textureの配列数
	depResourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //DepthStencilとして利用可能なフォーマット
	depResourceDesc.SampleDesc.Count = 1; //サンプリングカウント。1固定
	depResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D; //2次元
	depResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL; //DepthStencilとして使う通知

	//利用するHeapの設定
	D3D12_HEAP_PROPERTIES dsvHeapProperties{};
	dsvHeapProperties.Type = D3D12_HEAP_TYPE_DEFAULT; //VRAM上に作る

	//深度値のクリア設定
	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f; //1.0f(最大値)でクリア
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //フォーマット。Resourceと合わせる

	hr = DirectXCommon::GetInstance()->GetDevice()->CreateCommittedResource(
		&dsvHeapProperties, //Heapの設定
		D3D12_HEAP_FLAG_NONE, //Heapの特殊な設定。特になし。
		&depResourceDesc, //Resourceの設定
		D3D12_RESOURCE_STATE_DEPTH_WRITE, //深度値を書き込む状態にしておく
		&depthClearValue, //Clear最適地
		IID_PPV_ARGS(&depthBuff_) //作成するResourceポインタへのポインタ
	);
	assert(SUCCEEDED(hr));

	//DVSの設定
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT; //Format。基本的にはResourceに合わせる
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D; //2dTexture

	dsvHandleCPU_ = GetCPUDescriptorHandle(DirectXCommon::GetInstance()->GetDsvHeap(), DirectXCommon::GetInstance()->GetDevice()->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV), DirectXCommon::GetInstance()->GetDsvHeapCount());
	DirectXCommon::GetInstance()->IncrementDsvHeapCount();

	//DSVHeapの先頭にDSVを作る
	DirectXCommon::GetInstance()->GetDevice()->CreateDepthStencilView(depthBuff_.Get(), &dsvDesc, dsvHandleCPU_);

	//頂点バッファ生成
	vertexBuff_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(VertexData) * 4);
	//頂点バッファビューを作成する
	//リソースの先頭のアドレスから使う
	vertexBufferView_.BufferLocation = vertexBuff_->GetGPUVirtualAddress();
	//使用するリソースのサイズは頂点6つ分のサイズ
	vertexBufferView_.SizeInBytes = sizeof(VertexData) * 4;
	//1頂点あたりのサイズ
	vertexBufferView_.StrideInBytes = sizeof(VertexData);

	TransferVertex();

	indexBuff_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(uint32_t) * 6);

	indexBufferView_.BufferLocation = indexBuff_->GetGPUVirtualAddress();
	indexBufferView_.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView_.Format = DXGI_FORMAT_R32_UINT;

	uint32_t* indices = nullptr;
	indexBuff_->Map(0, nullptr, reinterpret_cast<void**>(&indices));

	indices[0] = 0;  indices[1] = 1;  indices[2] = 2;
	indices[3] = 1;  indices[4] = 3;  indices[5] = 2;

	materialBuff_ = CreateBufferResource(DirectXCommon::GetInstance()->GetDevice(), sizeof(MaterialData));
	materialBuff_->Map(0, nullptr, reinterpret_cast<void**>(&materialData_));
	materialData_->color_ = Vector4({ 1.0f,1.0f,1.0f,1.0f });

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