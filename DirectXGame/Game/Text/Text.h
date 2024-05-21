#pragma once
#include <vector>
#include "Camera.h"
#include "Object2dInstancing.h"

class Text
{
public:
	//配置タイプ
	enum ARRANGETYPE{
		//左揃え(デフォルト)
		kLeft,
		//中央ぞろえ
		kCenter
	};

	void Initialize();
	//表示文章をセット(毎フレーム呼ぶ)
	void SetText();
	//表示文字数変更
	void SetCharCount(uint32_t count) { showCharCount_ = count; };
	//改行するかどうか
	//void SetIsChangeLine(bool is) { isChangeLine_ = is; };
	//座標セット
	void SetPosition(const Vector2& pos) { origenPosition_ = pos; };
private:
	
	//表示文章上の各文字の幅
	float textStride_ = 64.0f;
	std::wstring text_ = L"互サ道サ道\nササぁあ道";

	//表示文字数
	uint32_t showCharCount_ = 0;

	//改行するか
	bool isChangeLine_ = false;
	//改行文字数
	uint32_t changeLineBorder_=32;

	ARRANGETYPE arrangeType_=kLeft;

	//文章の原点
	Vector2 origenPosition_ = {640.0f,360.0f};

};