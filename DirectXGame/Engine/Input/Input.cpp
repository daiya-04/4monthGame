#include "Input.h"

#include "WinApp.h"
#include <assert.h>
#include <cmath>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XInput.lib")

Input* Input::GetInstance() {
	static Input instance;

	return &instance;
}

void Input::Initialize() {

	HRESULT hr;

	//DirectInputの初期化
	hr = DirectInput8Create(WinApp::GetInstance()->GetHInstance(), DIRECTINPUT_VERSION, IID_IDirectInput8, (void**)&directInput, nullptr);
	assert(SUCCEEDED(hr));
	//キーボードデバイスの生成
	hr = directInput->CreateDevice(GUID_SysKeyboard, &keyBoard, NULL);
	assert(SUCCEEDED(hr));
	//入力データの	形式セット
	hr = keyBoard->SetDataFormat(&c_dfDIKeyboard); //標準形式
	assert(SUCCEEDED(hr));
	//排他制御レベルのセット
	hr = keyBoard->SetCooperativeLevel(WinApp::GetInstance()->GetHwnd(), DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	assert(SUCCEEDED(hr));

}

void Input::Update() {

	memcpy(preKey, key, sizeof(key));

	//キーボードの情報の取得開始
	keyBoard->Acquire();
	//全キーの入力状態を取得する
	keyBoard->GetDeviceState(sizeof(key), key);

	GetJoystickState();

}

bool Input::PushKey(BYTE keyNumber) const{

	if (key[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::TriggerKey(BYTE keyNumber) const{

	if (key[keyNumber] && !preKey[keyNumber]) {
		return true;
	}

	return false;
}

bool Input::GetJoystickState() {

	DWORD dwResult;

	preJoyState = joyState;

	dwResult = XInputGetState(0, &joyState);

	if (dwResult == ERROR_SUCCESS) {
		if (fabs(joyState.Gamepad.sThumbLX) < deadZone_) {
			joyState.Gamepad.sThumbLX = 0;
		}
		if (fabs(joyState.Gamepad.sThumbLY) < deadZone_) {
			joyState.Gamepad.sThumbLY = 0;
		}
		if (fabs(joyState.Gamepad.sThumbRX) < deadZone_) {
			joyState.Gamepad.sThumbRX = 0;
		}
		if (fabs(joyState.Gamepad.sThumbRY) < deadZone_) {
			joyState.Gamepad.sThumbRY = 0;
		}
		return true;
	}

	return false;
}

bool Input::TriggerLStick(Stick direction) {

	if (direction == Stick::Right) {
		return IsLStickRight();
	}
	if (direction == Stick::Left) {
		return IsLStickLeft();
	}
	if (direction == Stick::Up) {
		return IsLStickUp();
	}
	if (direction == Stick::Down) {
		return IsLStickDown();
	}
	return false;
}

bool Input::TiltLStick(Stick direction) {
	if (direction == Stick::Right) {
		return IsTiltRight();
	}
	if (direction == Stick::Left) {
		return IsTiltLeft();
	}
	if (direction == Stick::Up) {
		return IsTiltUp();
	}
	if (direction == Stick::Down) {
		return IsTiltDown();
	}
	return false;
}

bool Input::TriggerButton(Button button) const {
	if ((joyState.Gamepad.wButtons & (WORD)button) && !(preJoyState.Gamepad.wButtons & (WORD)button)) {
		return true;
	}
	return false;
}

bool Input::PushButton(Button button) const {
	if ((joyState.Gamepad.wButtons & (WORD)button)) {
		return true;
	}
	return false;
}

bool Input::ReleaseButton(Button button) const {
	if (!(joyState.Gamepad.wButtons & (WORD)button) && (preJoyState.Gamepad.wButtons & (WORD)button)) {
		return true;
	}
	return false;
}

bool Input::LeftTrigger() const {
	if (joyState.Gamepad.bLeftTrigger && !preJoyState.Gamepad.bLeftTrigger) {
		return true;
	}
	return false;
}

bool Input::RightTrigger() const {
	if (joyState.Gamepad.bRightTrigger && !preJoyState.Gamepad.bRightTrigger) {
		return true;
	}
	return false;
}

void Input::Vibration(float value) {

	value = std::clamp(value, 0.0f, 1.0f);

	XINPUT_VIBRATION vib{};
	
	//低周波
	vib.wLeftMotorSpeed = WORD(USHRT_MAX * value);
	//高周波
	vib.wRightMotorSpeed = WORD(USHRT_MAX * value);

	XInputSetState(0, &vib);
}

bool Input::IsLStickRight() const{

	if ((float)(joyState.Gamepad.sThumbLX) > deadZone_ && (float)(preJoyState.Gamepad.sThumbLX) <= deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsLStickLeft() const{
	if ((float)(joyState.Gamepad.sThumbLX) < -deadZone_ && (float)(preJoyState.Gamepad.sThumbLX) >= -deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsLStickUp() const{
	if ((float)(joyState.Gamepad.sThumbLY) > deadZone_ && (float)(preJoyState.Gamepad.sThumbLY) <= deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsLStickDown() const {
	if ((float)(joyState.Gamepad.sThumbLY) < -deadZone_ && (float)(preJoyState.Gamepad.sThumbLY) >= -deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsTiltRight() const {
	if ((float)(joyState.Gamepad.sThumbLX) > deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsTiltLeft() const {
	if ((float)(joyState.Gamepad.sThumbLX) < -deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsTiltUp() const {
	if ((float)(joyState.Gamepad.sThumbLY) > deadZone_) {
		return true;
	}
	return false;
}

bool Input::IsTiltDown() const {
	if ((float)(joyState.Gamepad.sThumbLY) < -deadZone_) {
		return true;
	}
	return false;
}
