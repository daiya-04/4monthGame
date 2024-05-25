#include "GameTextManager.h"
#include "TextureManager.h"
#include "ImGuiManager.h"
#include "DirectXCommon.h"
void GameTextManager::Initialize() {
	for (size_t i = 0; i < 9;i++) {
		nineSliceTextureBox_[i].reset(Sprite::Create(TextureManager::GetInstance()->Load("textBox.png"), Vector2{ 0,0 }, 9));
		nineSliceTextureBox_[i]->SetAnchorpoint({0.0f,0.0f});
	}
	nineSliceData_.position = {640.0f,600.0f};

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

void GameTextManager::Draw() {
	Sprite::preDraw(DirectXCommon::GetInstance()->GetCommandList());
	for (size_t i = 0; i < 9; i++) {
		nineSliceTextureBox_[i]->Draw();
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