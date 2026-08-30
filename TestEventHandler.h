#pragma once
#include "Entity.h"
#include "IEventHandler.h"
#include "IRenderWindow.h"
#include "Timer.h"
#include <Windows.h>
#include <bitset>
#include <memory>
#include <vector>

class TestEventHandler : public IEventHandler {
public:
	TestEventHandler() = delete;
	TestEventHandler(Timer, IRenderWindow&, std::vector<std::weak_ptr<Entity>>&);
	virtual void Register(UINT, WPARAM, POINT);
	virtual void Handle();
private:
	enum Key {
		KEY_UNKNOWN, KEY_ESC, KEY_Q, KEY_W, KEY_E, KEY_A, KEY_S, KEY_D, KEY_CTRL, KEY_SHIFT, KEY_SPACE, KEY_COUNT
	};
	std::bitset<TestEventHandler::Key::KEY_COUNT> _keyState;
	std::vector<std::weak_ptr<Entity>>& _entities;
	POINT _deltaCursorPosition{};
	POINT _defaultCursorPosition{};
	IRenderWindow& _window;
	Timer _timer;
};

