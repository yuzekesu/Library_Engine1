#pragma once
#include "CommandExecutor.h"
#include "DisplayManager.h"
#include "Entity.h"
#include "InputManager.h"
#include "MainWindowManager.h"
#include "TestEventHandler.h"
#include "Timer.h"
#include <array>
#include <memory>
#include <vector>

/// # DESCRIPTION
/// 1. All the abstracted game logic will be calculated here. 
/// # REMARK
/// 1. Never put any concrete calculation directly inside this class. Use other manager instead.
class GameManager {
public:
	GameManager();
	void Start();
private:
	DisplayManager _mainDisplay{ MainWindowManager::Instance(), {} };
	std::array<std::vector<std::weak_ptr<Entity>>, 10> _renderEntities;
	std::shared_ptr<TestEventHandler> _testEventHandler = std::make_shared<TestEventHandler>(Timer{ 1.f / 60.f }, _mainDisplay.Window(), _renderEntities[0]);
	InputManager _inputManager{ _testEventHandler };
};

