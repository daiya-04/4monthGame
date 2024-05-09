#pragma once
#include <d3d12.h>
#include <dxcapi.h>
#include <wrl.h>
#include "Vec2.h"
#include "Vec4.h"
#include "Matrix44.h"
#include <string>
#include <array>

class WaterDropPipeline
{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
	static void StaticInitialize(ID3D12Device* device, int windowWidth, int windowHeight);
	/// <summary>
	/// シェーダーのコンパイル
	/// </summary>
	/// <param name="filePath">CompilerするShaderファイルへのパス</param>
	/// <param name="profile">Compilerに使用するProfile</param>
	/// <param name="dxcUtils"></param>
	/// <param name="dxcCompiler"></param>
	/// <param name="includeHandleer"></param>
	/// <returns></returns>
	static ComPtr<IDxcBlob> CompileShader(const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils, IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandleer);

	static void preDraw(ID3D12GraphicsCommandList* commandList);

	static bool GetIsInitialized() { return isInitialized_; };

private:
	//共通部分の変数
	static ID3D12Device* device_;
	static ID3D12GraphicsCommandList* commandList_;
	static ComPtr<ID3D12RootSignature> rootSignature_;
	static ComPtr<ID3D12PipelineState> graphicsPipelineState_;

	static bool isInitialized_;//初期化処理が呼び出されたか
};
