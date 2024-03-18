#pragma once
#include <Windows.h>
#include <wrl.h>
#include "Vec3.h"
#include <string>

#include <XInput.h>
#define DIRECTINPUT_VERSION    0x0800
#include <dinput.h>

class Input{
private:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:

	ComPtr<IDirectInput8> directInput;
	ComPtr<IDirectInputDevice8> keyBoard;
	BYTE key[256] = {};
	BYTE preKey[256] = {};
	XINPUT_STATE joyState;
	XINPUT_STATE preJoyState;

public:

	static Input* GetInstance();

	//初期化
	void Initialize();
	//更新
	void Update();

	//キーの押下をチェック
	bool PushKey(BYTE keyNumber) const;

	//キーのトリガーをチェック
	bool TriggerKey(BYTE keyNumber) const;

	bool GetJoystickState();

	//Lスティックの傾きチェック
	bool TriggerLStick(const std::string& direction);

	

	Vector3 GetMoveXZ() {
		return { (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX };
	}

	Vector3 GetCameraRotate() {
		return { (float)joyState.Gamepad.sThumbRY / SHRT_MAX,(float)joyState.Gamepad.sThumbRX / SHRT_MAX,0.0f };
	}

	bool TriggerButton(int button) const {
		if ((joyState.Gamepad.wButtons & button) && !(preJoyState.Gamepad.wButtons & button)) {
			return true;
		}
		return false;
	}

	bool LeftTrigger() const;

	bool RightTrigger() const;

private:
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	bool IsLStickRight() const;

	bool IsLStickLeft() const;

	bool IsLStickUp() const;

	bool IsLStickDown() const;
};

