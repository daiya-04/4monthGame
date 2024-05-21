#pragma once
#include <vector>
#include "Camera.h"
#include "Object2dInstancing.h"

class Text
{
public:
	//配置タイプ
	enum arrangeType{
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
	void SetIsChangeLine(bool is) { isChangeLine_ = is; };
private:

	
	//表示文章上の各文字の幅
	float textStride_ = 64.0f;
	std::wstring text_ = L"互サ道サ道ササあ道";

	//表示文字数
	uint32_t showCharCount_ = 0;

	bool isChangeLine_ = false;

	//文章の原点
	Vector2 origenPosition_;
};