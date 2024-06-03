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
	phase_ = END;
	parametric_ = 0.0f;

	mainText_.reset(new Text);
	mainText_->Initialize();
	nameText_.reset(new Text);
	nameText_->Initialize();
	
	nextButton_.reset(Sprite::Create(TextureManager::GetInstance()->Load("AButton.png"), Vector2{ 0,0 }));
	nextButton_->SetScale(1.0f);

	skipButton_.reset(Sprite::Create(TextureManager::GetInstance()->Load("BButton.png"), Vector2{ 1.0f,1.0f }));
	skipButton_->SetAnchorpoint({0,0});
	skipText_.reset(Sprite::Create(TextureManager::GetInstance()->Load("skipText.png"), Vector2{ 1.0f,1.0f }));
	skipText_->SetAnchorpoint({ 0,0 });
	skipGaugeBack_.reset(Sprite::Create(TextureManager::GetInstance()->Load("skipGaugeBack.png"), Vector2{ 1.0f,1.0f }));
	skipGaugeBack_->SetAnchorpoint({ 0,0 });
	skipGauge_.reset(Sprite::Create(TextureManager::GetInstance()->Load("skipGauge.png"), Vector2{ 1.0f,1.0f }));
	skipGauge_->SetAnchorpoint({ 0,0 });

	buttonColor_ = { 1.0f,1.0f,1.0f,1.0f};

	std::string groupName = "TextLayout";

	GlobalVariables::GetInstance()->CreateGroup(dataName, groupName);

	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "TextBox-Position", nineSliceData_.position);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "TextBox-Size", textBoxOriginSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "NameBack-Position", nameBackPosition_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "NameBack-Size", nameBackSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "Name-Position", namePosition_);

	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "Button-Size", nextButtonSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "Button-Position", nextButtonOffset_);

	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipButton-Size", skipButtonSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipButton-Position", skipButtonPosition_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipText-Size", skipTextSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipText-Position", skipTextPosition_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipGauge-Size", skipGaugeSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipGauge-Position", skipGaugePosition_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipGaugeBack-Size", skipGaugeSize_);
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName, "SkipGaugeBack-Position", skipGaugePosition_);

	InitializeBackCharactors();

	AppryGlobalVariables();
	nameBack_->SetPosition(nameBackPosition_);
	nameBack_->SetSize(nameBackSize_);
	nameText_->SetPosition(namePosition_);
	nextButton_->SetSize(nextButtonSize_);

	skipButton_->SetPosition(skipButtonPosition_);
	skipButton_->SetSize(skipButtonSize_);
	skipText_->SetPosition(skipTextPosition_);
	skipText_->SetSize(skipTextSize_);
	skipGauge_->SetPosition(skipGaugePosition_);
	skipGauge_->SetSize(skipGaugeSize_);
	skipGauge_->SetColor({0.5f,0.5f,0.5f,1.0f});
	skipGaugeBack_->SetPosition(skipGaugeBackPosition_);
	skipGaugeBack_->SetSize(skipGaugeBackSize_);

	for (size_t i = 0; i < 3; i++) {
		backCharactors_[i].sprite->SetSize(backCharactorsSize_);
	}

	isEnd_ = true;
}

void GameTextManager::AppryGlobalVariables() {
	std::string groupName = "TextLayout";

	nineSliceData_.position = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "TextBox-Position");
	textBoxOriginSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "TextBox-Size");
	nameBackPosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "NameBack-Position");
	nameBackSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "NameBack-Size");
	namePosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "Name-Position");
	nextButtonSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "Button-Size");
	nextButtonOffset_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "Button-Position");

	skipButtonSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipButton-Size");
	skipButtonPosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipButton-Position");
	skipTextSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipText-Size");
	skipTextPosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipText-Position");
	skipGaugeSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipGauge-Size");
	skipGaugePosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipGauge-Position");
	skipGaugeBackSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipGaugeBack-Size");
	skipGaugeBackPosition_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName, "SkipGaugeBack-Position");
	
	std::string groupName2 = "TextLayout";

	backCharactors_[0].sprite->SetPosition(GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName2, "Position-Atsuo"));
	backCharactors_[1].sprite->SetPosition(GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName2, "Position-Samuo"));
	backCharactors_[2].sprite->SetPosition(GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName2, "Position-OldMan"));
	backCharactorsSize_ = GlobalVariables::GetInstance()->GetValue<Vector2>(dataName, groupName2, "BackCahactorsSize");
}

void GameTextManager::InitializeStage(uint32_t stageNum) {
	phase_ = OPEN;
	parametric_ = 0.0f;
	mainText_->Initialize();
	nameText_->Initialize();
	//文字列ロード処理入れる
	nameList_.clear();
	textList_.clear();
	LoadText(stageNum);
	
	listIndex_ = 0;
	if (!textList_.empty()) {
		mainText_->SetWString(textList_[listIndex_]);
		mainText_->SetCharCount(0);
		mainText_->SetCompleteDrawText(false);
		mainText_->SetPosition({ 200.0f,562.0f });

		nameText_->SetWString(nameList_[listIndex_]);
		nameText_->CharCountMax();
		nameText_->SetArrangeType(Text::kCenter);

		isEnd_ = false;
	}
	else {
		//リストが空の場合会話が終了したものとみなす
		mainText_->SetWString(L"");
		mainText_->SetCharCount(0);
		mainText_->SetCompleteDrawText(false);
		mainText_->SetPosition({ 200.0f,562.0f });

		nameText_->SetWString(L"");
		nameText_->CharCountMax();
		nameText_->SetArrangeType(Text::kCenter);
		phase_ = END;
		isEnd_ = true;
	}
	isSkip_ = false;
	skipButtonLength_ = 0;
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

void GameTextManager::Tutorial(int32_t tutorialNum) {
	phase_ = OPEN;
	parametric_ = 0.0f;
	nameList_.clear();
	textList_.clear();
	//各チュートリアルごとの文字列入れる
	switch (tutorialNum)
	{
	case 1:
		textList_.push_back(L"地下に無事行けたか！？");
		nameList_.push_back(L"老人");
		textList_.push_back(L"行けたぞ（あの爺さん声でかいな…）");
		nameList_.push_back(L"アツオ");
		textList_.push_back(L"てか…マグマか…？コレ…");
		nameList_.push_back(L"アツオ");
		textList_.push_back(L"もうそれだけ世界があったまってるということじゃ！");
		nameList_.push_back(L"老人");
		textList_.push_back(L"目の前のヘンテコなブロックが見えるか？");
		nameList_.push_back(L"老人");
		textList_.push_back(L"お、おう、あるな");
		nameList_.push_back(L"アツオ");
		textList_.push_back(L"それを壊すとマグマが少し下がる…だがじきに上がってくる！！");
		nameList_.push_back(L"老人");
		textList_.push_back(L"壊しながら下に進むしかないってことか…");
		nameList_.push_back(L"アツオ");
		textList_.push_back(L"そういうことじゃ！しばらく地下を掘ってみるんじゃ");
		nameList_.push_back(L"老人");
		break;
	case 2:
		textList_.push_back(L"なんかゲットしたぞ？赤い石？");
		nameList_.push_back(L"アツオ");
		textList_.push_back(L"それは鉱石じゃ！持っとくだけ得じゃ！");
		nameList_.push_back(L"老人");
		break;
	case 3:
		textList_.push_back(L"な、なんだったんだ…");
		nameList_.push_back(L"アツオ");
		textList_.push_back(L"マグマに触れたら戻されるのか…知らんかったのぉ…");
		nameList_.push_back(L"老人");
		break;
	case 4:
		textList_.push_back(L"右を見てみい、なにやら装置があるじゃろ？");
		nameList_.push_back(L"老人");
		textList_.push_back(L"集めた岩や鉱石でパワーアップできる！＊溜まってきたら試すといい");
		nameList_.push_back(L"老人");
		textList_.push_back(L"とりあえずコッチに帰ってこい");
		nameList_.push_back(L"老人");
		break;
	case 5:
		textList_.push_back(L"そしたら次は俺が行くかー？");
		nameList_.push_back(L"サムオ");
		textList_.push_back(L"そうじゃな、頼んだぞ");
		nameList_.push_back(L"老人");
		break;
	case 6:
		textList_.push_back(L"相棒の動きは見てたんじゃろ？さぁ黄金を目指して進め！");
		nameList_.push_back(L"老人");
		break;
	case 7:
		textList_.push_back(L"まぁ、こんな感じで交互に黄金を目指して掘ってくるんじゃ！");
		nameList_.push_back(L"老人");
		break;
	case 8:
		textList_.push_back(L"それじゃ！それが黄金じゃ！マグマに触れずに帰ってくるんじゃ！");
		nameList_.push_back(L"老人");
		break;
	default:
		break;
	}
	
	listIndex_ = 0;
	if (!textList_.empty()) {
		mainText_->SetWString(textList_[listIndex_]);
		mainText_->SetCharCount(0);
		mainText_->SetCompleteDrawText(false);
		mainText_->SetPosition({ 200.0f,562.0f });

		nameText_->SetWString(nameList_[listIndex_]);
		nameText_->CharCountMax();
		nameText_->SetArrangeType(Text::kCenter);

	}
	isEnd_ = false;
	isSkip_ = false; 
	skipButtonLength_ = 0;
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
	nextButton_->SetSize(nextButtonSize_);

	skipButton_->SetPosition(skipButtonPosition_);
	skipButton_->SetSize(skipButtonSize_);
	skipText_->SetPosition(skipTextPosition_);
	skipText_->SetSize(skipTextSize_);
	skipGauge_->SetPosition(skipGaugePosition_);
	skipGauge_->SetSize(skipGaugeSize_);
	skipGaugeBack_->SetPosition(skipGaugeBackPosition_);
	skipGaugeBack_->SetSize(skipGaugeBackSize_);
	for (size_t i = 0; i < 3; i++) {
		backCharactors_[i].sprite->SetSize(backCharactorsSize_);
	}
#endif // _DEBUG

	if (!isSkip_) {
		if (Input::GetInstance()->PushButton(Input::Button::B)) {
			skipButtonLength_++;
		}
		else {
			skipButtonLength_--;
		}
		if (skipButtonLength_ < 0) {
			skipButtonLength_ = 0;
		}
		if (skipButtonLength_ >= kSkipEx) {
			isSkip_ = true;
		}
		float t = float(skipButtonLength_) / float(kSkipEx);
		skipGauge_->SetTextureArea({ 0,0 }, {64*t,32.0f});
		skipGauge_->SetSize({skipGaugeSize_.x*t,skipGaugeSize_.y});
	}
	else {
		skipGauge_->SetSize(skipGaugeSize_);
	}


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
	parametric_ += 0.1f;
	if (parametric_ >= 1.0f) {
		parametric_ = 1.0f;
		phase_ = VIEW;
	}
	nextButtonTargetPosition_ = { 0.0f,0.0f };
	nextButtonOldPosition_ = nextButtonTargetPosition_;
	buttonParametric_ = 1.0f;
	for (size_t i = 0; i < 3; i++) {
		backCharactors_[i].isHighLight = false;
		backCharactors_[i].isShow = false;
		backCharactors_[i].sprite->SetScale(1.0f);
		backCharactors_[i].sprite->SetColor(Vector4{0.2f,0.2f,0.2f,1.0f});
	}
}

void GameTextManager::View() {
	nineSliceData_.size = textBoxOriginSize_;
	buttonColor_ = { 1.0f,1.0f,1.0f,1.0f };
	UpdateBackCharactors();
	if (!mainText_->GetCompleteDrawText()) {
		buttonColor_ = { 0.5f,0.5f,0.5f,1.0f };
		if (isSkip_) { mainText_->CharCountMax(); }
		//長押しで早送り
		if (Input::GetInstance()->PushButton(Input::Button::A)) {
			mainText_->SetCountUpFrame_(1);
		}
		else {
			mainText_->SetCountUpFrame_(5);
		}
		mainText_->CountUp();
	}
	//終了
	else if ((isSkip_ ) ||Input::GetInstance()->TriggerButton(Input::Button::A)) {
		//リストが残っていたらtextを差し替えてリセット
		if (listIndex_ < textList_.size()-1 && !textList_.empty()) {
			listIndex_++;
			mainText_->SetWString(textList_[listIndex_]);
			mainText_->SetCharCount(0);
			mainText_->SetCompleteDrawText(false);

			if (isSkip_) {mainText_->CharCountMax();}

			nameText_->SetWString(nameList_[listIndex_]);
			nameText_->CharCountMax();
			nameText_->SetArrangeType(Text::kCenter);
			
			for (size_t i = 0; i < 3; i++) {
				backCharactors_[i].isHighLight = false;
				backCharactors_[i].sprite->SetScale(1.0f);
				backCharactors_[i].sprite->SetColor(Vector4{ 0.2f,0.2f,0.2f,1.0f });
			}
			UpdateBackCharactors();
		}
		else {
			phase_ = CLOSE;
		}
	}

	//buttonanimation
	if (Input::GetInstance()->TriggerButton(Input::Button::A)) {
		nextButtonTargetPosition_ = {0.0f,6.0f};
		nextButtonOldPosition_ = nextButtonPosition_;
		buttonParametric_ = 0.0f;
	}
	else if (Input::GetInstance()->ReleaseButton(Input::Button::A)) {
		nextButtonTargetPosition_ = { 0.0f,0.0f };
		nextButtonOldPosition_ = nextButtonPosition_;
		buttonParametric_ = 0.0f;
	}
	if (buttonParametric_<=1.0f) {
		buttonParametric_ += 0.2f;
		nextButtonPosition_ = (nextButtonTargetPosition_ * buttonParametric_) + (1.0f- buttonParametric_) * nextButtonOldPosition_;
	}
	if (buttonParametric_ >= 1.0f) {
		buttonParametric_ = 1.0f;
	}
	nextButton_->SetPosition(nextButtonOffset_ + nextButtonPosition_);
	nextButton_->SetColor(buttonColor_);
	mainText_->SetText();
	nameText_->SetText();
	//next_->SetText();
}

void GameTextManager::Close() {
	nineSliceData_.size = textBoxOriginSize_ * Easing::easeOutBack(parametric_);
	parametric_ -= 0.1f;
	if (parametric_ < 0.0f) {
		parametric_ = 0.0f;
		nineSliceData_.size = { 0.0f,0.0f };
		phase_ = END;
	}
}

void GameTextManager::Draw() {
	if (phase_ != END && nineSliceData_.size != Vector2{0.0f,0.0f}) {
		Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
		if (phase_ == VIEW) {
			//notHighLight
			for (size_t i = 0; i < 3;i++) {
				if (backCharactors_[i].isShow && !backCharactors_[i].isHighLight) {
					backCharactors_[i].sprite->Draw();
				}
			}
			//highlight
			for (size_t i = 0; i < 3; i++) {
				if (backCharactors_[i].isHighLight) {
					backCharactors_[i].sprite->Draw();
				}
			}
		}
		for (size_t i = 0; i < 9; i++) {
			nineSliceTextureBox_[i]->Draw();
		}
		if (phase_ == VIEW) {
			nameBack_->Draw();
			nextButton_->Draw();

			skipGaugeBack_->Draw();
			skipGauge_->Draw();
			skipText_->Draw();
			skipButton_->Draw();
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

void GameTextManager::InitializeBackCharactors() {
	backCharactors_[0].sprite.reset(Sprite::Create(TextureManager::GetInstance()->Load("atsuo.png"), Vector2{ 0,0 }, 1.0f));
	backCharactors_[1].sprite.reset(Sprite::Create(TextureManager::GetInstance()->Load("samuo.png"), Vector2{ 0,0 }, 1.0f));
	backCharactors_[2].sprite.reset(Sprite::Create(TextureManager::GetInstance()->Load("oldMan.png"), Vector2{ 0,0 }, 1.0f));
	
	std::string groupName2 = "TextLayout";

	for (size_t i = 0; i < 3;i++) {
		backCharactors_[i].isHighLight = false;
		backCharactors_[i].isShow = false;
		backCharactors_[i].sprite->SetScale(1.0f);
		backCharactors_[i].sprite->SetAnchorpoint(Vector2{0.5f,1.0f});
	}
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName2, "Position-Atsuo", backCharactors_[0].sprite->GetPosition());
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName2, "Position-Samuo", backCharactors_[1].sprite->GetPosition());
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName2, "Position-OldMan", backCharactors_[2].sprite->GetPosition());
	GlobalVariables::GetInstance()->AddItem<const Vector2&>(dataName, groupName2, "BackCahactorsSize", backCharactorsSize_);
}
void GameTextManager::UpdateBackCharactors() {
	if (nameText_->GetWString().find(L"アツオ") != std::wstring::npos) {
		backCharactors_[0].isHighLight = true;
		backCharactors_[0].isShow = true;
		backCharactors_[0].sprite->SetScale(1.2f);
		backCharactors_[0].sprite->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
	}
	if(nameText_->GetWString().find(L"サムオ") != std::wstring::npos){
		backCharactors_[1].isHighLight = true;
		backCharactors_[1].isShow = true;
		backCharactors_[1].sprite->SetScale(1.2f);
		backCharactors_[1].sprite->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
	}
	if(nameText_->GetWString().find(L"老人") != std::wstring::npos) {
		backCharactors_[2].isHighLight = true;
		backCharactors_[2].isShow = true;
		backCharactors_[2].sprite->SetScale(1.2f);
		backCharactors_[2].sprite->SetColor(Vector4{ 1.0f,1.0f,1.0f,1.0f });
	}
}