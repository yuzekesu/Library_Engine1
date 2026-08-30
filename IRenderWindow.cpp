#include "IRenderWindow.h"

/// # DESCRIPTION
/// 1. Very default procedure.
/// 2. Only explicitly handle the WM_CLOSE message.
LRESULT IRenderWindow::_WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	switch (msg) {
	case WM_CLOSE:
		PostQuitMessage(0);
		break;
	default:
		break;
	}
	return DefWindowProcW(hwnd, msg, wparam, lparam);
}
/// # DESCRIPTION
/// 1. A simple getter function.
HWND IRenderWindow::HWnd() const noexcept {
	return this->_hwnd;
}
/// # DESCRIPTION
/// 1. A simple getter function.
int IRenderWindow::Width() const noexcept {
	return this->_rect.right;
}
/// # DESCRIPTION
/// 1. A simple getter function.
int IRenderWindow::Height() const noexcept {
	return this->_rect.bottom;
}
/// # DESCRIPTION
/// 1. A simple getter function.
int IRenderWindow::X() const noexcept {
	return this->_rect.left;
}
/// # DESCRIPTION
/// 1. A simple getter function.
int IRenderWindow::Y() const noexcept {
	return this->_rect.top;
}