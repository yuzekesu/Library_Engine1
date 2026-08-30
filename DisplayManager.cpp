#include "DirectX11Manager.h"
#include "DisplayManager.h"

/// # CONSTRUCTION
DisplayManager::DisplayManager(IRenderWindow& w, std::vector<std::unique_ptr<IRenderPipeline>> v) : _window{ w }, _directX11{ w, true }, _pipelines(std::move(v)) {
}
/// # DESCRIPTION
/// 1. A simple getter function.
IRenderWindow& DisplayManager::Window() noexcept {
	return this->_window;
}
/// # DESCRIPTION
/// 1. A simple getter function.
DirectX11Manager& DisplayManager::DirectX() noexcept {
	return this->_directX11;
}
/// # DESCRIPTION
/// 1. Replace the current render pipeline.
/// # REMARK
/// 1. Each pipeline contain it's own render objects, so replacing pipleine also replaces the render objects.
void DisplayManager::ReplacePipeline(std::vector<std::unique_ptr<IRenderPipeline>> p) {
	this->_pipelines = std::move(p);
}
/// # DESCRIPTION
/// 1. Render the frame for the specific window belong to this DisplayManager.
/// # REMARK
/// 1. The concrete implementation details of pipeline is not inside here.
void DisplayManager::RenderFrame() {
	// 🤓 clear the DSV before all the render pipelines.
	this->_directX11.DeferredContext().ClearDepthStencilView(&this->_directX11.DepthStencilView(), D3D11_CLEAR_DEPTH, 1.f, 0);
	for (auto& p : this->_pipelines) {
		p->Run(this->_window, this->_directX11);
	}
}
