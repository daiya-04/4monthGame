#include "TextManager.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include "Sprite.h"
void TextManager::Initialize() {
    fontOffsets_.clear();
    fontOffsets_.emplace(L"互", Vector2{ 1.0f,0});
    fontOffsets_.emplace(L"サ", Vector2{ 2.0f,0 });
    fontOffsets_.emplace(L"道", Vector2{ 3.0f,0 });
    fontOffsets_.emplace(L"ぁ", Vector2{ 3.0f,0 });
    drawObject_.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 512));
    //drawObject_->Init(1024);
    drawObject_->SetTextureHandle(TextureManager::Load("gameTextFont.png"));
    drawObject_->SetSize({ float(kTextStride_),float(kTextStride_) });
    testCamera_.reset(new Camera);
    testCamera_->Init();
    testCamera_->ChangeDrawingRange({ 1280.0f,720.0f });
    testCamera_->UpdateMatrix();
}

void TextManager::Draw() {
    Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
    drawObject_->Draw(*testCamera_.get());
    Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
}

void TextManager::TestDraw() {
    drawObject_->ClearUseCount();
    for (uint32_t index = 0; index < (text_.size() < showCharCount_ ? text_.size() : showCharCount_); index++) {
        //std::wstring ch = text_[index];
        auto it = fontOffsets_.find(text_.substr(index,  1));
        Vector2 texturePosition_ = {0,0};
        if (it != fontOffsets_.end()) {
            texturePosition_ = (it->second);
        }
        drawObject_->AppendObject({ 640.0f-96+kTextStride_*float(index),360.0f}, kCharStride_ * texturePosition_, {kCharStride_ ,kCharStride_});
    }
    Object2dInstancing::preDraw(DirectXCommon::GetInstance()->GetCommandList());
    drawObject_->Draw(*testCamera_.get());
    Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
}

void TextManager::AppendChar(const Vector2& position,const std::wstring& str) {
    //std::wstring ch = text_[index];
    auto it = fontOffsets_.find(str);
    Vector2 texturePosition_ = { 0,0 };
    if (it != fontOffsets_.end()) {
        texturePosition_ = (it->second);
    }
    drawObject_->AppendObject(position, kCharStride_ * texturePosition_, { kCharStride_ ,kCharStride_ });
}