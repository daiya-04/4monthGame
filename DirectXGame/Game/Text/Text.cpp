#include "Text.h"

void Text::Initialize() {

}

void Text::SetText() {
    for (uint32_t index = 0; index < (text_.size() < showCharCount_ ? text_.size() : showCharCount_); index++) {
        //std::wstring ch = text_[index];
        /*auto it = fontOffsets_.find(text_.substr(index, 1));
        Vector2 texturePosition_ = { 0,0 };
        if (it != fontOffsets_.end()) {
            texturePosition_ = (it->second);
        }*/
        //drawObject_->AppendObject({ 640.0f - 96 + kTextStride_ * float(index),360.0f }, kCharStride_ * texturePosition_, { kCharStride_ ,kCharStride_ });
    }
}