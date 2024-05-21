#pragma once
#include <map>
#include <string>
#include <Vec2.h>
#include <memory>
//#include <d2d1.h>
//#include <d2d1helper.h>
//#include <dwrite.h>
//#include <wrl.h>
#include <vector>
#include "Camera.h"
#include "Object2dInstancing.h"

class TextManager
{
public:

	//インスタンス取得
	static TextManager* GetInstance() {
		static TextManager instancee;
		return &instancee;
	};



	void Initialize();
	void TestDraw();
	//表示文字数変更
	void SetCharCount(uint32_t count) { showCharCount_ = count; };
private:
	//first:文字 second:テクスチャ左上座標
	std::map<std::wstring, Vector2> fontOffsets_;

	//画像上の各文字の幅
	const float kCharStride_ = 32.0f;
	//表示文章上の各文字の幅
	const float kTextStride_ = 64.0f;
	std::wstring text_ = L"互サ道サ道ササあ道";

	std::unique_ptr<Camera> testCamera_;//spriteをinstancing対応させるまでの仮カメラ
	
	//表示用オブジェクト
	std::unique_ptr<Object2dInstancing> drawObject_;

	//表示文字数
	uint32_t showCharCount_=0;
};
