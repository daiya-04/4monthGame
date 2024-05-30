#include "WinApp.h"
#include <imgui_impl_win32.h>

#include "resource.h"

#pragma comment(lib,"winmm.lib")

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);


WinApp* WinApp::GetInstance() {
	static WinApp instance;

	return &instance;
}

LRESULT WinApp::WinProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {

	if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam)) {
		return true;
	}

	//メッセージに応じてゲーム固有の処理を行う
	switch (msg) {
		//ウィンドウが破壊された
	case WM_DESTROY:
		//OSに対して、アプリの終了を伝える
		PostQuitMessage(0);
		return 0;
	}

	//標準のメッセージ処理を行う
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinApp::CreateGameWindow(const wchar_t* title, int32_t clientWidth, int32_t clientHeight) {

	title_ = title;

	//システムタイマーの分解機能を上げる
	timeBeginPeriod(1);

	//COMの初期化
	CoInitializeEx(0, COINIT_MULTITHREADED);

	//ウィンドウプロシージャ
	wc_.lpfnWndProc = WinProc;
	//ウィンドウクラス名
	wc_.lpszClassName = L"DirectXGame";
	//インスタンスハンドル
	wc_.hInstance = GetModuleHandle(nullptr);
	//カーソル
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);

	wc_.hIcon = LoadIcon(wc_.hInstance, MAKEINTRESOURCE(IDI_ICON1));

	//ウィンドウクラスを登録する
	RegisterClass(&wc_);

	windowStyle_ = WS_OVERLAPPEDWINDOW;

	//ウィンドウサイズを表す構造体にクライアント領域を入れる
	windowRect_ = { 0,0,clientWidth,clientHeight };
	//クライアント領域を元に実際のサイズにrectを変更してもらう
	AdjustWindowRect(&windowRect_, windowStyle_, false);

	hwnd_ = CreateWindow(
		wc_.lpszClassName,
		title_,
		WS_OVERLAPPEDWINDOW,
		120,
		50,
		windowRect_.right - windowRect_.left,
		windowRect_.bottom - windowRect_.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr
	);

	
	ShowWindow(hwnd_, SW_SHOW);
}

bool WinApp::ProcessMessage() {
	MSG msg{};

	if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	if (msg.message == WM_QUIT) {
		return true;
	}

	return false;
}

void WinApp::TerminateGameWindow() {
	CoUninitialize();
	CloseWindow(hwnd_);
}

void WinApp::ChangeScreenMode(ScreenMode screenMode) {

	switch (screenMode) {
		case ScreenMode::kWindow:
			// 通常のウィンドウに戻す
			SetWindowLong(hwnd_, GWL_STYLE, windowStyle_);
			SetWindowPos(
				hwnd_,
				NULL,
				120, 50,
				windowRect_.right - windowRect_.left,
				windowRect_.bottom - windowRect_.top,
				SWP_FRAMECHANGED | SWP_SHOWWINDOW
			);
			isFullScreen_ = false;
			break;
		case ScreenMode::kFullScreen:
			// 通常ウィンドウの時の状態を保存
			windowStyle_ = GetWindowLong(hwnd_, GWL_STYLE);
			GetWindowRect(hwnd_, &windowRect_);

			// フルスクリーンにする
			SetWindowLong(hwnd_, GWL_STYLE, WS_POPUP);
			SetWindowPos(hwnd_, HWND_TOP, 0, 0,
				GetSystemMetrics(SM_CXSCREEN),
				GetSystemMetrics(SM_CYSCREEN),
				SWP_FRAMECHANGED | SWP_SHOWWINDOW
			);
			isFullScreen_ = true;
			break;
	}

}
