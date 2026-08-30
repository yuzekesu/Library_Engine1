#include "MainWindowManager.h"
#include <Windows.h>
#define WINDOW_NAME L"Testing..."

/// # SINGLETON
/// 1. Helper function to check the existing instance.
/// # RETURN
/// 1. The created/exsiting instance.
MainWindowManager& MainWindowManager::Instance() {
	static MainWindowManager the_only_main_window;
	return the_only_main_window;
}
/// # CONSTRUCTION
/// 1. The HWND and WNDClass registration.
/// 2. Shows the window upon success.
/// # REMARK
/// 1. The window title name is defined as a MACRO at the top of this document.
/// 2. The size of the window is currently fixed.
MainWindowManager::MainWindowManager() {
	wchar_t NAME[] = WINDOW_NAME;
	// const DWORD windowStyle = WS_OVERLAPPED  | WS_SYSMENU;
	const DWORD windowStyle = WS_POPUP;
	{
		auto& r = this->_rect;
		const int screenX = GetSystemMetrics(SM_CXSCREEN);
		const int screenY = GetSystemMetrics(SM_CYSCREEN);
		//r.right = screenX / 3 * 2;
		//r.bottom = screenY / 3 * 2;
		//r.left = (screenX - r.right) / 2;
		//r.top = (screenY - r.bottom) / 2;
		//AdjustWindowRect(&r, windowStyle, false);
		r.right = screenX;
		r.bottom = screenY;
		r.left = 0;
		r.top = 0;
	}
	{
		auto& c = this->_wndClass;
		c.cbClsExtra = 0;
		c.cbSize = sizeof(c);
		c.cbWndExtra = 0;
		// the nullptr is needed for transparent background.
		c.hbrBackground = nullptr;
		c.hCursor = LoadCursorW(NULL, IDC_ARROW);
		c.hIcon = LoadIconW(NULL, IDC_NO);
		c.hIconSm = NULL;
		c.hInstance = GetModuleHandleW(NULL);
		c.lpfnWndProc = MainWindowManager::_WndProc;
		c.lpszClassName = NAME;
		c.lpszMenuName = NAME;
		c.style = NULL;
		RegisterClassExW(&c);
	}
	{
		const int WIDTH = this->_rect.right;
		const int HEIGHT = this->_rect.bottom;
		const int X = this->_rect.left;
		const int Y = this->_rect.top;
		// the WS_EX_NOREDIRECTIONBITMAP is needed for transparent background.
		this->_hwnd = CreateWindowExW(WS_EX_NOREDIRECTIONBITMAP, NAME, NAME, windowStyle, X, Y, WIDTH, HEIGHT, NULL, NULL, this->_wndClass.hInstance, NULL);
		ShowWindow(this->_hwnd, SW_SHOW);
	}
}


