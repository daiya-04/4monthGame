#include "Scroll.h"

Scroll::Scroll()
{
}

Scroll::~Scroll()
{
}

void Scroll::Initialize() {

	startScroll_ = { limitStart_.x + camera_->center_.x, limitStart_.y + camera_->center_.y };

	//xScroll
	type_[0] = kDelay;
	//yScroll
	type_[1] = kDelay;

}

void Scroll::SetCameraOnTarget() {

	if (target_ && camera_) {
		camera_->translation_.x = target_->x - camera_->center_.x;
		camera_->translation_.y = target_->y - camera_->center_.y;
	}

}

void Scroll::Update() {

	if (target_ && camera_) {

		//スクロール開始地点より左だったら
		if (target_->x < startScroll_.x) {

			//カメラのX座標を0にする
			switch (type_[0])
			{
			default:
			case kNomral:

				camera_->translation_.x += limitStart_.x - camera_->translation_.x;

				break;

			case kDelay:

				stalkerSpeed_ = std::clamp(stalkerSpeed_, 0.00001f, 1.0f);

				camera_->translation_.x += (limitStart_.x - camera_->translation_.x) * stalkerSpeed_;

				break;

			}

		}
		//スクロール終了地点より右だったら
		else if (target_->x > limitEnd_.x) {

			
			//カメラのX座標を限界値にする
			switch (type_[0])
			{
			default:
			case kNomral:

				camera_->translation_.x += limitEnd_.x - camera_->center_.x - camera_->translation_.x;

				break;

			case kDelay:

				stalkerSpeed_ = std::clamp(stalkerSpeed_, 0.00001f, 1.0f);

				camera_->translation_.x += (limitEnd_.x - camera_->center_.x - camera_->translation_.x) * stalkerSpeed_;

				break;

			}

		}
		//スクロール範囲内ならターゲット座標に追従
		else {

			switch (type_[0])
			{
			default:
			case kNomral:

				camera_->translation_.x += target_->x - camera_->center_.x - camera_->translation_.x;

				break;

			case kDelay:

				stalkerSpeed_ = std::clamp(stalkerSpeed_, 0.00001f, 1.0f);

				camera_->translation_.x += (target_->x - camera_->center_.x - camera_->translation_.x) * stalkerSpeed_;

				break;

			}

		}

		//スクロール開始地点より上だったら
		if (target_->y < startScroll_.y) {

			//カメラのY座標を0にする
			switch (type_[1])
			{
			default:
			case kNomral:

				camera_->translation_.y += limitStart_.y - camera_->translation_.y;

				break;

			case kDelay:

				stalkerSpeed_ = std::clamp(stalkerSpeed_, 0.00001f, 1.0f);

				camera_->translation_.y += (limitStart_.y - camera_->translation_.y) * stalkerSpeed_;

				break;

			}

		}
		//スクロール終了地点より下だったら
		else if (target_->y > limitEnd_.y) {

			//カメラのY座標を限界値にする
			switch (type_[1])
			{
			default:
			case kNomral:

				camera_->translation_.y += limitEnd_.y - camera_->center_.y - camera_->translation_.y;

				break;

			case kDelay:

				stalkerSpeed_ = std::clamp(stalkerSpeed_, 0.00001f, 1.0f);

				camera_->translation_.y += (limitEnd_.y - camera_->center_.y - camera_->translation_.y) * stalkerSpeed_;

				break;

			}

		}
		//スクロール範囲内ならターゲット座標に追従
		else {

			switch (type_[1])
			{
			default:
			case kNomral:

				camera_->translation_.y += target_->y - camera_->center_.y - camera_->translation_.y;

				break;

			case kDelay:

				stalkerSpeed_ = std::clamp(stalkerSpeed_, 0.00001f, 1.0f);

				camera_->translation_.y += (target_->y - camera_->center_.y - camera_->translation_.y) * stalkerSpeed_;

				break;

			}

		}

	}

}
