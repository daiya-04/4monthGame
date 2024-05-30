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

	//文字送りを実行する(毎フレーム呼び出し)
	void CountUp();

	//表示文章をセット(毎フレーム呼ぶ)
	void SetText();
	//表示文字数変更
	void SetCharCount(size_t count) { showCharCount_ = count; };

	//文字を全部表示する
	void CharCountMax() {
		showCharCount_ = text_.size();
		isCompleteDrawText_ = true;
	};

	//改行するかどうか
	//void SetIsChangeLine(bool is) { isChangeLine_ = is; };
	//座標セット
	void SetPosition(const Vector2& pos) { origenPosition_ = pos; };

	//文章をセットする
	void SetWString(const std::wstring& newStr) { text_ = newStr; };

	const std::wstring& GetCurrentText() { return text_; }

	void SetCountUpFrame_(int32_t s) { countUpFrame_ = s; };

	void SetCompleteDrawText(bool is) { isCompleteDrawText_ = is; };

	bool GetCompleteDrawText() { return isCompleteDrawText_; };

	void SetArrangeType(ARRANGETYPE t) { arrangeType_ = t; };

private:
	
	//表示文章上の各文字の幅
	float textStride_ = 32.0f;
	std::wstring text_;

	//表示文字数
	size_t showCharCount_ = 0;

	//改行するか
	bool isChangeLine_ = false;
	//改行文字数
	size_t changeLineBorder_=32;

	ARRANGETYPE arrangeType_=kLeft;

	//文章の原点
	Vector2 origenPosition_ = {640.0f,360.0f};

	//何フレームで文字送りするか
	int32_t countUpFrame_;

	//内部カウント
	int32_t count_;

	//文字送りが完了したか
	bool isCompleteDrawText_;
};