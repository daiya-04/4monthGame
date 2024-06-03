#include "Text.h"
#include "TextManager.h"
void Text::Initialize() {
    count_ = 0;
    voice1_ = AudioManager::GetInstance()->Load("SE/voice_01.mp3");
    voice2_ = AudioManager::GetInstance()->Load("SE/voice_02.mp3");
    voice3_ = AudioManager::GetInstance()->Load("SE/voice_03.mp3");
    voice4_ = AudioManager::GetInstance()->Load("SE/voice_04.mp3");
    voice5_ = AudioManager::GetInstance()->Load("SE/voice_05.mp3");
    voice6_ = AudioManager::GetInstance()->Load("SE/voice_06.mp3");
}

void Text::CountUp() {
    if (!isCompleteDrawText_) {
        count_++;
        if (count_ > countUpFrame_) {
            count_ = 0;
            showCharCount_++;

            switch (static_cast<int32_t>(text_[showCharCount_]) % 6 + 1)
            {
            default:
            case 1:
                voice1_->Play();
                break;
            case 2:
                voice2_->Play();
                break;
            case 3:
                voice3_->Play();
                break;
            case 4:
                voice4_->Play();
                break;
            case 5:
                voice5_->Play();
                break;
            case 6:
                voice6_->Play();
                break;
            }

        }
        //文字数が最大になったら止める
        if (showCharCount_ >= text_.size()) {
            showCharCount_ = uint32_t(text_.size());
            isCompleteDrawText_ = true;
        }
        else {
            isCompleteDrawText_ = false;
        }
    }
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
            txtPosition.x += - textStride_ * float(text_.size() < showCharCount_ ? text_.size() : showCharCount_)*0.5f + (textStride_ * float(index));
            break;
        default:
            break;
        }
       
        if (arrangeType_ == kLeft && (text_.substr(index, 1) == L"\n" || text_.substr(index, 1) == L"＊")) {
            lineCount++;
            widthCount = 0;
        }
        else if (text_.substr(index, 1) == L"＋") {
            widthCount++;
        }
        else {
            TextManager::GetInstance()->AppendChar(txtPosition, text_.substr(index, 1));
            widthCount++;
        }
    }
}