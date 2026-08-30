#pragma once
#include "IRenderWindow.h"

/// # DESCRIPTION
/// 1. Singleton class that manage the window related attribute.
/// # SEEALSO
/// 1. WindowManger parent class.
class MainWindowManager : public IRenderWindow {
public:
	static MainWindowManager& Instance();
	MainWindowManager(const MainWindowManager&) = delete;
	MainWindowManager(MainWindowManager&&) = delete;
	MainWindowManager& operator=(const MainWindowManager&) = delete;
	MainWindowManager& operator=(MainWindowManager&&) = delete;
private:
	MainWindowManager();
	~MainWindowManager() = default;
};

