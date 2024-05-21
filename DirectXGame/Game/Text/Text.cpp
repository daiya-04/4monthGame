#include "Text.h"
#include "TextManager.h"
void Text::Initialize() {

}

void Text::SetText() {
    //改行カウント
    uint32_t lineCount=0;
    //横文字数カウント(改行でクリア)
    uint32_t widthCount=0;
    for (uint32_t index = 0; index < (text_.size() < showCharCount_ ? text_.size() : showCharCount_); index++) {
        Vector2 txtPosition = origenPosition_;
        switch (arrangeType_)
        {
        case Text::kLeft:
            txtPosition.x += textStride_ * float(widthCount);
            txtPosition.y += textStride_ * float(lineCount);
            break;
        case Text::kCenter:
            txtPosition.x -= textStride_ * float(text_.size() < showCharCount_ ? text_.size() : showCharCount_)*0.5f + (textStride_ * float(index));
            break;
        default:
            break;
        }
       
        if (arrangeType_ == kLeft && text_.substr(index, 1) == L"\n") {
            lineCount++;
            widthCount = 0;
        }
        else {
            TextManager::GetInstance()->AppendChar(txtPosition, text_.substr(index, 1));
            widthCount++;
        }
    }
}