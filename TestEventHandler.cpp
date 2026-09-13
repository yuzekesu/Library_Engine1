#include "TestEventHandler.h"
#include <Windows.h>

TestEventHandler::TestEventHandler(Timer t, IRenderWindow& w, std::vector<std::weak_ptr<Entity>>& vector) : _timer{ t }, _window{ w }, _entities{ vector } {
}

void TestEventHandler::Register(UINT uMsg, WPARAM wParam, POINT point) {
	TestEventHandler::Key k = KEY_UNKNOWN;
	switch (wParam) {
	case VK_ESCAPE:
		k = TestEventHandler::Key::KEY_ESC;
		break;
	case 'Q':
		k = TestEventHandler::Key::KEY_Q;
		break;
	case 'W':
		k = TestEventHandler::Key::KEY_W;
		break;
	case 'E':
		k = TestEventHandler::Key::KEY_E;
		break;
	case 'A':
		k = TestEventHandler::Key::KEY_A;
		break;
	case 'S':
		k = TestEventHandler::Key::KEY_S;
		break;
	case 'D':
		k = TestEventHandler::Key::KEY_D;
		break;
	case VK_SPACE:
		k = TestEventHandler::Key::KEY_SPACE;
		break;
	case VK_SHIFT:
		k = TestEventHandler::Key::KEY_SHIFT;
		break;
	default:
		break;
	}
	RECT rect{};
	switch (uMsg) {
	case WM_KEYDOWN:
		this->_keyState[k] = true;
		break;
	case WM_KEYUP:
		this->_keyState[k] = false;
		break;
	case WM_SETFOCUS:
		_deltaCursorPosition.x += point.x - _defaultCursorPosition.x;
		_deltaCursorPosition.y += point.y - _defaultCursorPosition.y;
		GetClientRect(_window.HWnd(), &rect);
		// Client-space center, matching WM_MOUSEMOVE coordinates.
		_defaultCursorPosition.x = (rect.right - rect.left) / 2;
		_defaultCursorPosition.y = (rect.bottom - rect.top) / 2;
		// Convert rect to screen coordinates for SetCursorPos/ClipCursor.
		MapWindowPoints(_window.HWnd(), nullptr, reinterpret_cast<POINT*>(&rect), 2);
		SetCursorPos(rect.left + (rect.right - rect.left) / 2, rect.top + (rect.bottom - rect.top) / 2);
		break;
	case WM_MOUSEMOVE:
		ShowCursor(FALSE); // hide
		if (point.x == _defaultCursorPosition.x &&
			point.y == _defaultCursorPosition.y) {
			break;
		}
		_deltaCursorPosition.x += point.x - _defaultCursorPosition.x;
		_deltaCursorPosition.y += point.y - _defaultCursorPosition.y;
		GetClientRect(_window.HWnd(), &rect);
		// Client-space center, matching WM_MOUSEMOVE coordinates.
		_defaultCursorPosition.x = (rect.right - rect.left) / 2;
		_defaultCursorPosition.y = (rect.bottom - rect.top) / 2;
		// Convert rect to screen coordinates for SetCursorPos/ClipCursor.
		MapWindowPoints(_window.HWnd(), nullptr, reinterpret_cast<POINT*>(&rect), 2);
		SetCursorPos(rect.left + (rect.right - rect.left) / 2, rect.top + (rect.bottom - rect.top) / 2);
		ClipCursor(&rect);
		break;
	}
}

void TestEventHandler::Handle() {
	const float time = this->_timer.Seconds();
	const float distance = 1.f * time;
	const float angle = 90.f * time;
	if (this->_entities.size() == 0) return;
	auto pEntity = this->_entities[0].lock();
	pEntity->Transform().Turn(this->_deltaCursorPosition.y / 20.f, this->_deltaCursorPosition.x / 20.f, 0.f);
	this->_deltaCursorPosition = {};
	if (_keyState[KEY_ESC]) PostQuitMessage(0);
	if (_keyState[KEY_Q]) pEntity->Transform().Turn(0.f, 0.f, angle);
	if (_keyState[KEY_W]) pEntity->Transform().Move(RigidTransform::Direction::FORWARD, distance);
	if (_keyState[KEY_E]) pEntity->Transform().Turn(0.f, 0.f, -angle);
	if (_keyState[KEY_A]) pEntity->Transform().Move(RigidTransform::Direction::LEFT, distance);
	if (_keyState[KEY_S]) pEntity->Transform().Move(RigidTransform::Direction::BACK, distance);
	if (_keyState[KEY_D]) pEntity->Transform().Move(RigidTransform::Direction::RIGHT, distance);
	if (_keyState[KEY_SHIFT]) pEntity->Transform().Move(RigidTransform::Direction::DOWN, distance);
	if (_keyState[KEY_SPACE]) pEntity->Transform().Move(RigidTransform::Direction::UP, distance);
}
