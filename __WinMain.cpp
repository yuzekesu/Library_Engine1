#include "GameManager.h"
#include <Windows.h>
#include <exception>

/// # MAIN ENTRY POINT
int WINAPI wWinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ PWSTR pCmdLine, _In_ int nCmdShow) {
	try {
		GameManager manager{};
		manager.Start();
	}
	catch (const std::exception& e) {
		MessageBoxA(NULL, e.what(), "Exception", NULL);
	}
	return 0;
}