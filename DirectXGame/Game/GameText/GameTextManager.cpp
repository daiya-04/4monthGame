#include "GameTextManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "DirectXCommon.h"
#include "Easing.h"
#include "Input.h"
#include "GlobalVariables.h"
#include "Log.h"
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
void GameTextManager::Initialize() {
	for (size_t i = 0; i < 9;i++) {
		nineSliceTextureBox_[i].reset(Sprite::Create(TextureManager::GetInstance()->Load("textBox.png"), Vector2{ 0,0 }, 9));
		nineSliceTextureBox_[i]->SetAnchorpoint({0.0f,0.0f});
		nineSliceTextureBox_[i]->SetScale({1.0f,1.0f});
	}

	nameBack_.reset(Sprite::Create(TextureManager::GetInstance()->Load("nameBack.png"), Vector2{ 0,0 }));


	nineSliceData_.position = {640.0f,600.0f};
	textBoxOriginSize_ = {1000.0f,150.0f};
	phase_ = OPEN;
	parametric_ = 0.0f;

	mainText_.reset(new Text);
	mainText_->Initialize();
	nameText_.reset(new Text);
	nameText_->Initialize();
	next_.reset(new Text);
	next_->Initialize();

	std::string groupName = "TextLayout";

	GlobalVariables::GetInstance()->CreateGroup(dataName, groupName);

	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "TextBox-Position", nineSliceData_.position);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "TextBox-Size", textBoxOriginSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "NameBack-Position", nameBackPosition_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "NameBack-Size", nameBackSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "Name-Position", namePosition_);

	AppryGlobalVariables();
	nameBack_->SetPosition(nameBackPosition_);
	nameBack_->SetSize(nameBackSize_);
	nameText_->SetPosition(namePosition_);
}

void GameTextManager::AppryGlobalVariables() {
	std::string groupName = "TextLayout";

	nineSliceData_.position = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "TextBox-Position");
	textBoxOriginSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "TextBox-Size");
	nameBackPosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "NameBack-Position");
	nameBackSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "NameBack-Size");
	namePosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "Name-Position");
}

void GameTextManager::InitializeStage(uint32_t stageNum) {
	phase_ = OPEN;
	parametric_ = 0.0f;
	//文字列ロード処理入れる
	//LoadText(stageNum);
	//textList_.push_back(L"ササササササササササササササササササササササササ\nササササササササササササササササササササササササ");
	//textList_.push_back(L"互サ道互サ道互サ道互サ道互サ道互サ道互サ道互サ道");
	listIndex_ = 0;
	if (!textList_.empty()) {
		mainText_->SetWString(textList_[listIndex_]);
		mainText_->SetCharCount(0);
		mainText_->SetCompleteDrawText(false);
		mainText_->SetPosition({ 200.0f,562.0f });

		nameText_->SetWString(nameList_[listIndex_]);
		nameText_->CharCountMax();
		nameText_->SetArrangeType(Text::kCenter);

		next_->SetWString(L"A");
		next_->SetPosition({ 1100.0f,650.0f });
		next_->SetCharCount(1);
	}
	isEnd_ = false;
}

void GameTextManager::LoadText(uint32_t stageNum) {

	std::string fileName = "./Resources/TextData/";
	fileName += "text";
	fileName += std::to_string(stageNum);
	fileName += ".text";
	std::wstring line;
	std::wifstream file(fileName);
	std::wstring name;
	std::wstring text;
	//ファイル読み込みが出来なかったら処理を止める
	if (!file.is_open()) {
		//MessageBox(nullptr, L"ファイルを読み込めませんでした。", L"FontOffsets", 0);
		return;
	}
	
	while (std::getline(file, line)) {
		std::wistringstream s(line);
		s >> text >> name;
		nameList_.push_back(name);
		textList_.push_back(text);
	}
	file.close();
}

void GameTextManager::TestUpdate() {
#ifdef _DEBUG
	static float scale;

	ImGui::Begin("TextBox");
	ImGui::DragFloat2("center", &nineSliceData_.position.x, 1.0f);
	ImGui::DragFloat2("originSize", &textBoxOriginSize_.x, 1.0f);
	ImGui::DragFloat("scale", &scale, 0.1f,0.0f,1.5f);
	ImGui::End();
	nineSliceData_.size = textBoxOriginSize_ * scale;
#endif // _DEBUG
	SetNineSliceData();
}

void GameTextManager::Update() {
#ifdef _DEBUG
	AppryGlobalVariables();
	nameBack_->SetPosition(nameBackPosition_);
	nameBack_->SetSize(nameBackSize_);
	nameText_->SetPosition(namePosition_);
#endif // _DEBUG

	switch (phase_)
	{
	case GameTextManager::OPEN:
		Open();
		break;
	case GameTextManager::VIEW:
		View();
		break;
	case GameTextManager::CLOSE:
		Close();
		break;
	case GameTextManager::END:
		isEnd_ = true;
		break;
	default:
		break;
	}
	//座標セット
	SetNineSliceData();
}

void GameTextManager::Open() {
	nineSliceData_.size = textBoxOriginSize_ * Easing::easeOutBack(parametric_);
	parametric_ += 0.05f;
	if (parametric_ >= 1.0f) {
		parametric_ = 1.0f;
		phase_ = VIEW;
	}
}

void GameTextManager::View() {
	nineSliceData_.size = textBoxOriginSize_;
	if (!mainText_->GetCompleteDrawText()) {
		//長押しで早送り
		if (Input::GetInstance()->PushButton(Input::Button::A)) {
			mainText_->SetCountUpFrame_(0);
		}
		else {
			mainText_->SetCountUpFrame_(5);
		}
		mainText_->CountUp();
	}
	//終了
	else if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		//リストが残っていたらtextを差し替えてリセット
		if (listIndex_ < textList_.size()-1 && !textList_.empty()) {
			listIndex_++;
			mainText_->SetWString(textList_[listIndex_]);
			mainText_->SetCharCount(0);
			mainText_->SetCompleteDrawText(false);

			nameText_->SetWString(nameList_[listIndex_]);
			nameText_->CharCountMax();
			nameText_->SetArrangeType(Text::kCenter);
		}
		else {
			phase_ = CLOSE;
		}
	}
	mainText_->SetText();
	nameText_->SetText();
	next_->SetText();
}

void GameTextManager::Close() {
	nineSliceData_.size = textBoxOriginSize_ * Easing::easeOutBack(parametric_);
	parametric_ -= 0.05f;
	if (parametric_ < 0.0f) {
		parametric_ = 0.0f;
		nineSliceData_.size = { 0.0f,0.0f };
		phase_ = END;
	}
}

void GameTextManager::Draw() {
	if (phase_ != END && nineSliceData_.size != Vector2{0.0f,0.0f}) {
		Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		for (size_t i = 0; i < 9; i++) {
			nineSliceTextureBox_[i]->Draw();
		}
		if (phase_ == VIEW) {
			nameBack_->Draw();
		}
	}
}

void GameTextManager::SetNineSliceData() {
	//box座標計算
	Vector2 leftTop = nineSliceData_.position - (nineSliceData_.size * 0.5f);
	Vector2 rightTop = nineSliceData_.position + (nineSliceData_.size * 0.5f);
	rightTop.x -= nineSliceData_.partSize.x;
	rightTop.y = leftTop.y;
	Vector2 centerSize = nineSliceData_.size - (nineSliceData_.partSize * 2.0f);
	Vector2 textureCenterSize = nineSliceData_.textureSize - (nineSliceData_.texturePartSize * 2.0f);

	//左上
	nineSliceTextureBox_[0]->SetPosition(leftTop);
	nineSliceTextureBox_[0]->SetSize(nineSliceData_.partSize);
	nineSliceTextureBox_[0]->SetTextureArea({ 0.0f,0.0f }, nineSliceData_.texturePartSize);

	//中上
	nineSliceTextureBox_[1]->SetPosition(leftTop + Vector2{ nineSliceData_.partSize.x,0.0f });
	nineSliceTextureBox_[1]->SetSize({ nineSliceData_.size.x - (nineSliceData_.partSize.x * 2.0f), nineSliceData_.partSize.y });
	nineSliceTextureBox_[1]->SetTextureArea({nineSliceData_.texturePartSize.x,0.0f}, nineSliceData_.textureSize - (nineSliceData_.texturePartSize * 2.0f));

	//右上
	nineSliceTextureBox_[2]->SetPosition(rightTop);
	nineSliceTextureBox_[2]->SetSize(nineSliceData_.partSize);
	nineSliceTextureBox_[2]->SetTextureArea(Vector2{nineSliceData_.textureSize.x - nineSliceData_.texturePartSize.x,0.0f}, nineSliceData_.texturePartSize);



	//左中
	nineSliceTextureBox_[3]->SetPosition(leftTop + Vector2{0.0f,nineSliceData_.partSize.y});
	nineSliceTextureBox_[3]->SetSize(Vector2{ nineSliceData_.partSize.x,centerSize .y});
	nineSliceTextureBox_[3]->SetTextureArea({ 0.0f,nineSliceData_.texturePartSize.y }, { nineSliceData_.texturePartSize.x,textureCenterSize.y});

	//中中
	nineSliceTextureBox_[4]->SetPosition(leftTop + nineSliceData_.partSize);
	nineSliceTextureBox_[4]->SetSize(centerSize);
	nineSliceTextureBox_[4]->SetTextureArea(nineSliceData_.texturePartSize, textureCenterSize);

	//右中
	nineSliceTextureBox_[5]->SetPosition(rightTop + Vector2{ 0.0f,nineSliceData_.partSize.y });
	nineSliceTextureBox_[5]->SetSize(Vector2{ nineSliceData_.partSize.x,centerSize .y});
	nineSliceTextureBox_[5]->SetTextureArea(nineSliceData_.texturePartSize + Vector2{textureCenterSize.x,0.0f }, { nineSliceData_.texturePartSize.x,textureCenterSize.y });



	//左下
	nineSliceTextureBox_[6]->SetPosition(leftTop + Vector2{0.0f, nineSliceData_.partSize.y + centerSize.y });
	nineSliceTextureBox_[6]->SetSize(nineSliceData_.partSize);
	nineSliceTextureBox_[6]->SetTextureArea({ 0.0f,textureCenterSize.y + nineSliceData_.texturePartSize.y }, nineSliceData_.texturePartSize);

	//中下
	nineSliceTextureBox_[7]->SetPosition(leftTop + Vector2{ nineSliceData_.partSize.x,0.0f } + Vector2{ 0.0f, nineSliceData_.partSize.y + centerSize.y });
	nineSliceTextureBox_[7]->SetSize({ nineSliceData_.size.x - (nineSliceData_.partSize.x * 2.0f), nineSliceData_.partSize.y });
	nineSliceTextureBox_[7]->SetTextureArea(Vector2{ nineSliceData_.partSize.x,textureCenterSize.y + nineSliceData_.texturePartSize.y }, { nineSliceData_.textureSize.x - (nineSliceData_.texturePartSize.x * 2.0f),nineSliceData_.texturePartSize.y });

	//右下
	nineSliceTextureBox_[8]->SetPosition(rightTop + Vector2{ 0.0f, nineSliceData_.partSize.y + centerSize.y });
	nineSliceTextureBox_[8]->SetSize(nineSliceData_.partSize);
	nineSliceTextureBox_[8]->SetTextureArea(Vector2{ nineSliceData_.textureSize.x - nineSliceData_.texturePartSize.x,textureCenterSize.y + nineSliceData_.texturePartSize.y }, nineSliceData_.texturePartSize);
}