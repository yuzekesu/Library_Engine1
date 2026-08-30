#pragma once
#include "DirectX11Manager.h"
#include "IRenderPipeline.h"
#include "IRenderWindow.h"
#include <memory>
#include <vector>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

/// # DESCRIPTION
/// 1. It has all display related APIs here.
/// # REMARK
/// 1. This class is per HWND, per ID3D11Device.
/// 2. Each DisplayManager class has its own display pipelines.
/// 3. The pipelines will start from the end.
class DisplayManager {
public:
	DisplayManager() = delete;
	DisplayManager(IRenderWindow&, std::vector<std::unique_ptr<IRenderPipeline>>);
	IRenderWindow& Window() noexcept;
	DirectX11Manager& DirectX() noexcept;
	void ReplacePipeline(std::vector<std::unique_ptr<IRenderPipeline>>);
	void RenderFrame();
private:
	IRenderWindow& _window;
	DirectX11Manager _directX11;
	std::vector<std::unique_ptr<IRenderPipeline>> _pipelines;
};

