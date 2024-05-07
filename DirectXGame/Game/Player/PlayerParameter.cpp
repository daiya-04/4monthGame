#include "PlayerParameter.h"

void PlayerParameter::Initialize() {

	Jump_.canJump = false;
	Jump_.graceTime = 0;
	Jump_.jumpVelocity = -25.0f;

	wallJump_.canWallJump = false;
	wallJump_.graceTime = 0;
	wallJump_.wallJumpVelocity = { 10.0f,-30.0f };

	dig_.isDig = false;
	dig_.digInterval = 42;
	dig_.digCount = 0;

	chargeJump_.isCharge = false;
	chargeJump_.chargeTimer = 0;
	chargeJump_.chargeJumpVelocity = -35.0f;
	chargeJump_.canBreak = false;

	saunaTimer_.maxSaunaTime = 3600;
	saunaTimer_.countSaunaTimer = saunaTimer_.maxSaunaTime;

}

void PlayerParameter::CopyParameter(const PlayerParameter& origin) {

	speed_ = origin.speed_;
	maxMoveSpeed_ = origin.maxMoveSpeed_;

	Jump_.kMaxGraceTime = origin.Jump_.kMaxGraceTime;
	Jump_.jumpVelocity = origin.Jump_.jumpVelocity;

	wallJump_.kMaxGraceTime = origin.wallJump_.kMaxGraceTime;
	wallJump_.wallJumpVelocity = origin.wallJump_.wallJumpVelocity;

	dig_.digInterval = origin.dig_.digInterval;

	chargeJump_.maxChargeTime = origin.chargeJump_.maxChargeTime;
	chargeJump_.chargeJumpVelocity = origin.chargeJump_.chargeJumpVelocity;
	chargeJump_.jumpValue = origin.chargeJump_.jumpValue;

	saunaTimer_.maxSaunaTime = origin.saunaTimer_.maxSaunaTime;

}
