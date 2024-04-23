#pragma once
#include <Windows.h>
#include <cstdint>

class WinApp{
private:
	WinApp() = default;
	~WinApp() = default;
	WinApp(const WinApp&) = delete;
	WinApp& operator=(const WinApp&) = delete;


	WNDCLASS wc_{};
	HWND hwnd_ = nullptr;

	const wchar_t* title_;
	RECT windowRect_{};
	UINT windowStyle_;

public: //静的メンバ変数
	static const int32_t kClientWidth = 1280;
	static const int32_t kClientHeight = 720;

	enum class ScreenMode {
		kWindow,
		kFullScreen,
	};

public: //静的メンバ関数

	static WinApp* GetInstance();

	static LRESULT WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
public: //メンバ関数
	HWND GetHwnd() const { return hwnd_; }

	HINSTANCE GetHInstance() const { return wc_.hInstance; }

	void CreateGameWindow(const wchar_t* title = L"DirectXGame", int32_t clientWidth = kClientWidth, int32_t clientHeight = kClientHeight);

	bool ProcessMessage();

	void TerminateGameWindow();

	void ChangeScreenMode(ScreenMode screenMode);

};

