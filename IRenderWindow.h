#pragma once
#include <Windows.h>

/// # ABSTRACT
/// 1. Represent a window that owns all related APIs.
/// # REMARK
/// 1. Each window shall be reprecented by one concrete IRenderWindow instance.
class IRenderWindow {
public:
	virtual ~IRenderWindow() = default;
	HWND HWnd() const noexcept;
	int Width() const noexcept;
	int Height() const noexcept;
	int X() const noexcept;
	int Y() const noexcept;
protected:
	IRenderWindow() = default;
	static LRESULT CALLBACK _WndProc(HWND, UINT, WPARAM, LPARAM);
	WNDCLASSEXW _wndClass{};
	HWND _hwnd = NULL;
	RECT _rect{};
};