#include "IRenderWindow.h"
#include "InputManager.h"
#include <Windows.h>
#include <format>
#include <initializer_list>
#include <stdexcept>
#include <windowsx.h>

InputManager::InputManager(std::initializer_list<std::weak_ptr<IEventHandler>> list) : _registeredEvent(list) {}

const POINT& InputManager::MousePosition() const noexcept {
	return InputManager::_mousePosition;
}

void InputManager::New(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	if (wParam > 256) throw std::invalid_argument(std::format("VK exceeding the upperbound. WPARAM: {}", wParam));
	POINT& p = InputManager::_mousePosition;
	switch (uMsg) {
	case WM_MOUSEMOVE:
		p.x = GET_X_LPARAM(lParam);
		p.y = GET_Y_LPARAM(lParam);
		[[fallthrough]];
	case WM_SETFOCUS:
	case WM_KEYDOWN:
	case WM_KEYUP:
		for (auto& weak : this->_registeredEvent) {
			auto share = weak.lock();
			if (!share) continue;
			auto& event = *share;
			event.Register(uMsg, wParam, p);
		}
		break;
	default:
		throw std::invalid_argument(std::format("Unexpected WM: {}", uMsg));
	}
}

void InputManager::HandleInputs() {
	for (auto& weak : this->_registeredEvent) {
		auto share = weak.lock();
		if (!share) continue;
		auto& event = *share;
		event.Handle();
	}
}

void InputManager::SetEventHandlers(std::initializer_list<std::weak_ptr<IEventHandler>> vector) {
	this->_registeredEvent = std::move(vector);
}

