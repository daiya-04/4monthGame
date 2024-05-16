#pragma once
#include <map>
#include <string>
#include <Vec2.h>

class TextManager
{
public:

private:
	//first:文字 second:テクスチャ左上座標
	std::map<std::wstring, Vector2> fontOffsets_;

	//各文字の幅
	static const float kTextStride_ = 32.0f;
};
