#pragma once
#include <Windows.h>
#include <wrl.h>
#include "Vec2.h"
#include "Vec3.h"
#include <string>

#include <XInput.h>
#define DIRECTINPUT_VERSION    0x0800
#include <dinput.h>

class Input{
private:

	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:

	enum class Button {
		DPAD_UP = 0x0001,
		DPAD_DOWN = 0x0002,
		DPAD_LEFT = 0x0004,
		DPAD_RIGHT = 0x0008,
		START = 0x0010,
		BACK = 0x0020,
		LEFT_THUMB = 0x0040,
		RIGHT_THUMB = 0x0080,
		LEFT_SHOULDER = 0x0100,
		RIGHT_SHOULDER = 0x0200,
		A = 0x1000,
		B = 0x2000,
		X = 0x4000,
		Y = 0x8000,
	};

	enum class Trigger {
		Left,
		Right,
	};

	enum class Stick {
		Left,
		Right,
		Up,
		Down,
	};

private:

	ComPtr<IDirectInput8> directInput;
	ComPtr<IDirectInputDevice8> keyBoard;
	BYTE key[256] = {};
	BYTE preKey[256] = {};
	XINPUT_STATE joyState;
	XINPUT_STATE preJoyState;
	SHORT deadZone_ = 10000;

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
	bool TriggerLStick(Stick direction);

	bool TiltLStick(Stick directon);

	float GetLX() const { return (float)joyState.Gamepad.sThumbLX / SHRT_MAX; }

	float GetLY() const { return (float)joyState.Gamepad.sThumbLY / SHRT_MAX; }

	Vector2 GetLXY() const { return { GetLX(),GetLY() }; }

	Vector3 GetMoveXZ() const {
		return { (float)joyState.Gamepad.sThumbLX / SHRT_MAX, 0.0f, (float)joyState.Gamepad.sThumbLY / SHRT_MAX };
	}

	Vector3 GetCameraRotate() const {
		return { (float)joyState.Gamepad.sThumbRY / SHRT_MAX,(float)joyState.Gamepad.sThumbRX / SHRT_MAX,0.0f };
	}

	//パッドボタンのトリガーをチェック
	bool TriggerButton(Button button) const;
	//パッドボタンの押下をチェック
	bool PushButton(Button button) const;
	//パッドボタンのリリース(離し)のチェック
	bool ReleaseButton(Button button) const;

	bool LeftTrigger() const;

	bool RightTrigger() const;

	void Vibration(float value);

private:
	Input() = default;
	~Input() = default;
	Input(const Input&) = delete;
	Input& operator=(const Input&) = delete;

	bool IsLStickRight() const;

	bool IsLStickLeft() const;

	bool IsLStickUp() const;

	bool IsLStickDown() const;

	bool IsTiltRight() const;

	bool IsTiltLeft() const;

	bool IsTiltUp() const;

	bool IsTiltDown() const;
};

