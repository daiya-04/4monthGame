#include "TextManager.h"
#include "TextureManager.h"
#include "DirectXCommon.h"
#include "Sprite.h"
#include "ImGuiManager.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
void TextManager::Initialize() {
    std::locale::global(std::locale(""));
    fontOffsets_.clear();
    LoadFontOffset();
   // fontOffsets_.emplace(L"互", Vector2{ 1.0f,0});
   // fontOffsets_.emplace(L"サ", Vector2{ 2.0f,0 });℃
   // fontOffsets_.emplace(L"道", Vector2{ 3.0f,0 });
   // fontOffsets_.emplace(L"ぁ", Vector2{ 3.0f,0 });
    drawObject_.reset(Object2dInstancing::Create(0, Vector2{ 0,0 }, 512));
    //drawObject_->Init(1024);
    drawObject_->SetTextureHandle(TextureManager::Load("text.png"));
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
        drawObject_->AppendObject({ 640.0f-96+kTextStride_*float(index),360.0f},0, kCharStride_ * texturePosition_, {kCharStride_ ,kCharStride_},0);
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
    Vector4 col = {1.0f,1.0f,1.0f,1.0f};
    
    drawObject_->AppendObject(position, { float(kTextStride_),float(kTextStride_) },0, kCharStride_ * texturePosition_, { kCharStride_ ,kCharStride_ },col,0);
}

void TextManager::LoadFontOffset() {
    std::string fileName = "./Resources/FontData/offsetData.fontOffset";
    std::wstring line;
    std::wifstream file(fileName);
    std::wstring key;
    Vector2 position;
    //ファイル読み込みが出来なかったら処理を止める
    if (!file.is_open()) {
        MessageBox(nullptr, L"ファイルを読み込めませんでした。", L"FontOffsets", 0);
        return;
    }
    fontOffsets_.clear();
    while (std::getline(file, line)) {
        std::wistringstream s(line);
        s >> key >> position.x >> position.y;
        fontOffsets_.emplace(key,position);
    }
    file.close();
}

void TextManager::SaveFontOffset() {
    std::string fileName = "./Resources/FontData/offsetData.fontOffset";
    std::wstring line;
    std::wofstream file(fileName);
    //ファイル読み込みが出来なかったら処理を止める
    if (file.fail()) {
        MessageBox(nullptr, L"ファイルを読み込めませんでした。", L"FontOffsets", 0);
        return;
    }

  //  auto Loc = std::locale(Locale.c_str());
   // auto L = file.imbue(Loc);

    for (auto & offset : fontOffsets_) {
        file << offset.first << " " << offset.second.x << " " << offset.second.y << std::endl;
    }

    file.close();
    std::string message = "Saved FontOffsets !";
    MessageBoxA(nullptr, message.c_str(), "TextManager", 0);
}

void TextManager::OffsetEditorDraw() {
#ifdef _DEBUG

    ImGui::Begin("FontOffset");

    if (ImGui::Button("SaveFile")) {
        //セーブする
        SaveFontOffset();
    }
    static char text[8] = "";
    ImGui::InputText("key", text, sizeof(text), 8);
    static Vector2 offset;
    ImGui::DragFloat2("position", &offset.x, 1.0f);
    if (ImGui::Button("Add")) {
        //文字オフセット追加
        auto it = fontOffsets_.find(ConvertString(text));
        //既存のkeyだったら値を変更
        if (it != fontOffsets_.end()) {
            fontOffsets_[it->first] = offset;
        }
        //無ければ追加する
        else {
            fontOffsets_.emplace(ConvertString(text), offset);
        }
    }
    ImGui::SameLine();
    //キー削除
    if (ImGui::Button("Remove")) {
        auto it = fontOffsets_.find(ConvertString(text));
        if (it != fontOffsets_.end()) {
            fontOffsets_.erase(it->first);
        }
    }
    ImGui::Text(" ");
    //一覧表示
    for (auto& offset : fontOffsets_) {
        ImGui::DragFloat2(ConvertString(offset.first).c_str(), &offset.second.x);
    }

    ImGui::End();
#endif // _DEBUG
}