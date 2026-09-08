#pragma once
#include "IEventHandler.h"
#include <Windows.h>
#include <initializer_list>
#include <memory>
#include <vector>

class InputManager {
public:
	InputManager() = delete;
	InputManager(std::initializer_list<std::weak_ptr<IEventHandler>> list = {});
	const POINT& MousePosition() const noexcept;
	void Register(UINT, WPARAM, LPARAM);
	void Handle();
	void Set(std::initializer_list<std::weak_ptr<IEventHandler>>);
private:
	POINT _mousePosition{};
	std::vector<std::weak_ptr<IEventHandler>> _registeredEvent;
};

