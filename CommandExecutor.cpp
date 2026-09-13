#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#include "CommandExecutor.h"

Render::CommandExecutor::CommandExecutor(ID3D11DeviceContext& immediate) : _immediate(immediate) {
	// render thread.
	// 1. run as long 
	this->_renderThread = std::jthread([this](std::stop_token stopToken) {
		while (true) {
			Render::CommandExecutor::Submission submission;
			{
				std::unique_lock lock(this->_renderQueueMutex);
				this->_renderQueueChanged.wait(lock, stopToken, [this] {return !this->_renderQueue.empty(); });
				if (stopToken.stop_requested()) break;
				submission = std::move(this->_renderQueue.front());
				this->_renderQueue.pop();
			}
			this->_immediate.ExecuteCommandList(submission.pCommandList.Get(), FALSE);
			submission.pSwapChain->Present(1, 0);
		}
		});
}
/// # DESCRIPTION
/// 1. As the method name is saying.
/// # REMARK
/// 1. Do not forget to call it in the worker thread.
/// 2. No visual updates if not calling this for that thread/window.
void Render::CommandExecutor::Submit(ComPtr<ID3D11CommandList> cl, ComPtr<IDXGISwapChain4> sc) {
	std::lock_guard lock(this->_renderQueueMutex);
	this->_renderQueue.push({ std::move(cl), std::move(sc) });
	this->_renderQueueChanged.notify_one();
}
