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
