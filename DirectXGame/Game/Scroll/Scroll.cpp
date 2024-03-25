#include "Scroll.h"

Scroll::Scroll()
{
}

Scroll::~Scroll()
{
}

void Scroll::Initialize() {

}

void Scroll::Update() {

	if (target_ && camera_) {

		//スクロール開始地点より左だったら
		if (target_->x < startScroll_.x) {

			//カメラのX座標を0にする
			camera_->translation_.x = 0.0f;

		}
		//スクロール終了地点より右だったら
		else if (target_->x > limit_.x) {

			//カメラのX座標を限界値にする
			camera_->translation_.x = limit_.x - startScroll_.x;

		}
		//スクロール範囲内ならターゲット座標に追従
		else {

			camera_->translation_.x = target_->x - startScroll_.x;

		}

		//スクロール開始地点より上だったら
		if (target_->y < startScroll_.y) {

			//カメラのY座標を0にする
			camera_->translation_.y = 0.0f;

		}
		//スクロール終了地点より下だったら
		else if (target_->y > limit_.y) {

			//カメラのY座標を限界値にする
			camera_->translation_.y = limit_.y - startScroll_.y;

		}
		//スクロール範囲内ならターゲット座標に追従
		else {

			camera_->translation_.y = target_->y - startScroll_.y;

		}

	}

}
