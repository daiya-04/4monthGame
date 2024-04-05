#include "Scroll.h"

Scroll::Scroll()
{
}

Scroll::~Scroll()
{
}

void Scroll::Initialize() {

	startScroll_ = { limitStart_.x + kWindowCenter_.x, limitStart_.y + kWindowCenter_.y };

}

void Scroll::Update() {

	if (target_ && camera_) {

		//スクロール開始地点より左だったら
		if (target_->x < startScroll_.x) {

			//カメラのX座標を0にする
			camera_->translation_.x = limitStart_.x;

		}
		//スクロール終了地点より右だったら
		else if (target_->x > limitEnd_.x) {

			//カメラのX座標を限界値にする
			camera_->translation_.x = limitEnd_.x - kWindowCenter_.x;

		}
		//スクロール範囲内ならターゲット座標に追従
		else {

			camera_->translation_.x = target_->x - kWindowCenter_.x;

		}

		//スクロール開始地点より上だったら
		if (target_->y < startScroll_.y) {

			//カメラのY座標を0にする
			camera_->translation_.y = limitStart_.y;

		}
		//スクロール終了地点より下だったら
		else if (target_->y > limitEnd_.y) {

			//カメラのY座標を限界値にする
			camera_->translation_.y = limitEnd_.y - kWindowCenter_.y;

		}
		//スクロール範囲内ならターゲット座標に追従
		else {

			camera_->translation_.y = target_->y - kWindowCenter_.y;

		}

	}

}
