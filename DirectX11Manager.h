#pragma once
#include "IRenderWindow.h"
#include <d3d11.h>
#include <dcomp.h>
#include <dxgi1_6.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

/// # DESCRIPTION
/// Singleton. Simple to access factory, adapter, device and deviceContext.
class DirectX11Manager {
public:
	DirectX11Manager() = delete;
	DirectX11Manager(IRenderWindow&, bool isMainManager = false);
	DirectX11Manager(const DirectX11Manager&) = delete;
	DirectX11Manager(DirectX11Manager&&) = delete;
	DirectX11Manager& operator=(const DirectX11Manager&) = delete;
	DirectX11Manager& operator=(DirectX11Manager&&) = delete;
	ID3D11DeviceContext& DeferredContext() noexcept;
	IDXGISwapChain4& SwapChain() noexcept;
	ComPtr<IDXGISwapChain4>& ComPtrSwapChain() noexcept;
	ID3D11RenderTargetView& RTV() noexcept;
	const D3D11_VIEWPORT& Viewport() noexcept;
	IDCompositionDesktopDevice& CompositionDevice() noexcept;
	ID3D11DepthStencilView& DepthStencilView() noexcept;
	static void InitializeStatics();
	static IDXGIFactory6& Factory() noexcept;
	static IDXGIAdapter4& Adapter() noexcept;
	static ID3D11Device& Device() noexcept;
	static ID3D11DeviceContext& ImmediateContext() noexcept;
private:
	bool _isMainManager = false;
	D3D11_VIEWPORT _viewport{};
	ComPtr<ID3D11DeviceContext> _deferredContext;
	ComPtr<IDXGISwapChain4> _swapchain;
	ComPtr<ID3D11RenderTargetView> _rtv;
	ComPtr<IDCompositionDesktopDevice> _compositionDevice;
	ComPtr<IDCompositionTarget> _target;
	ComPtr<IDCompositionVisual3> _visual;
	ComPtr<ID3D11Texture2D> _depthBuffer;
	ComPtr<ID3D11DepthStencilView> _depthStencilView;
	inline static bool _areStaticsInitialized = false;

	inline static ComPtr<IDXGIFactory6> _factory;
	inline static ComPtr<IDXGIAdapter4> _adapter;
	inline static ComPtr<ID3D11Device> _device;
	inline static ComPtr<ID3D11DeviceContext> _immediateContext;
};

