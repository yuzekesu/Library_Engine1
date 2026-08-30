#include "DirectX11Manager.h"
#include "Entity.h"
#include "GameManager.h"
#include "TestPipelineManager.h"
#include "Timer.h"
#include <memory>


GameManager::GameManager() {
	static auto camera = std::make_shared<Entity>(L"cube");
	static std::vector<std::shared_ptr<Entity>> staticVector;
	this->_renderEntities[0].push_back(camera);
	for (int i = 0; i < 10; ++i) {
		for (int u = 0; u < 10; ++u) {
			auto cube = std::make_shared<Entity>(L"cube");
			cube->Move(IMovable::Direction::BACK, 1.5f * 5);
			cube->Move(IMovable::Direction::LEFT, 1.5f * 5);
			cube->Move(IMovable::Direction::FORWARD, 1.5f * i);
			cube->Move(IMovable::Direction::RIGHT, 1.5f * u);
			staticVector.push_back(cube);
			this->_renderEntities[0].push_back(cube);
		}
	}
	std::vector<std::unique_ptr<IRenderPipeline>> piplines{};
	piplines.push_back(std::make_unique<TestPipelineManager>(this->_renderEntities[0]));
	this->_mainDisplay.ReplacePipeline({ std::move(piplines) });
}

/// # DESCRIPTION
/// 1. Start the game.
/// # REMARK
/// 1. The main logic loop of the whole application is here.
void GameManager::Start() {
	Timer timer(1.f / 60.f);
	MSG msg;
	while (true) {
		auto share = this->_renderEntities[0][1].lock();
		if (share)share->Rotate(0.f, 1.f, 0.f);


		// 🔞 Never use If-statement for PeekMessageW.
		// 🔞 Because the message may accumulate.
		// 🔞 Which will make the input feels laggy.
		while (PeekMessageW(&msg, NULL, NULL, NULL, PM_REMOVE)) {
			if (msg.message == WM_QUIT) return;
			DispatchMessageW(&msg);
			if (msg.message == WM_SETFOCUS || msg.message == WM_KEYDOWN || msg.message == WM_KEYUP || msg.message == WM_MOUSEMOVE) {
				this->_inputManager.New(msg.message, msg.wParam, msg.lParam);
			}
		}
		this->_mainDisplay.RenderFrame();
		this->_inputManager.HandleInputs();
		timer.Wait();
	}
}
