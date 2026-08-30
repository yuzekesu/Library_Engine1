#pragma once
#include <Windows.h>

class IEventHandler {
public:
	virtual ~IEventHandler() = default;
	virtual void Register(UINT, WPARAM, POINT) = 0;
	virtual void Handle() = 0;
private:
};

