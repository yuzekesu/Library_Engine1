#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dcomp.lib")
#include "DirectX11Manager.h"
#include "Exception.h"
#include <array>
#include <mutex>

/// # CONSTRUCTION
/// 1. statics
/// 2. deferred Context
/// 3. SwapChain.
/// 4. RenderTargetView.
/// 5. CompositionDesktopDevice.
/// 6. CompositionTarget.
/// 7. CompositionVisual.
/// 8. Set the CompositionTarget and CompositionVisual and then commit the changes.
DirectX11Manager::DirectX11Manager(IRenderWindow& w, bool isMainManager) : _isMainManager(isMainManager) {
	this->InitializeStatics();
	HRESULT hr;

	// viewport
	{
		auto& v = this->_viewport;
		v.Height = static_cast<float>(w.Height());
		v.MaxDepth = 1.f;
		v.MinDepth = 0.f;
		v.Width = static_cast<float>(w.Width());
	}

	// deferred context
	hr = DirectX11Manager::_device->CreateDeferredContext(0, this->_deferredContext.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(hr);

	// swapchain.
	{
		ComPtr<IDXGISwapChain1> newSwapchain;
		DXGI_SWAP_CHAIN_DESC1 d{};
		d.AlphaMode = DXGI_ALPHA_MODE_PREMULTIPLIED;
		d.BufferCount = 2;
		d.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		d.Flags = 0;
		d.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		d.Height = w.Height();
		d.SampleDesc.Count = 1;
		d.SampleDesc.Quality = 0;
		d.Scaling = DXGI_SCALING_STRETCH;
		d.Stereo = FALSE;
		d.SwapEffect = DXGI_SWAP_EFFECT_FLIP_SEQUENTIAL;
		d.Width = w.Width();
		hr = this->_factory->CreateSwapChainForComposition(DirectX11Manager::_device.Get(), &d, nullptr, newSwapchain.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw Exception{ hr };
		hr = newSwapchain->QueryInterface(IID_PPV_ARGS(this->_swapchain.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw Exception{ hr };
	}

	// rtv.
	{
		ComPtr<ID3D11Resource> r;
		hr = this->_swapchain->GetBuffer(0, IID_PPV_ARGS(r.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw Exception{ hr };
		D3D11_RENDER_TARGET_VIEW_DESC d{};
		hr = DirectX11Manager::_device->CreateRenderTargetView(r.Get(), nullptr, this->_rtv.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw Exception{ hr };
	}

	// compDevice.
	// how is IDCompositionDesktopDevice created from DCompositionCreateDevice3.
	// damn you Microsoft.
	hr = DCompositionCreateDevice3(NULL, IID_PPV_ARGS(this->_compositionDevice.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) throw Exception{ hr };

	// compose target & visual.
	hr = this->_compositionDevice->CreateTargetForHwnd(w.HWnd(), TRUE, this->_target.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception{ hr };
	// idk why we cannot create the IDCompositionVisual3 directly Microsoft.
	ComPtr<IDCompositionVisual2> newVisual;
	hr = this->_compositionDevice->CreateVisual(newVisual.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception{ hr };
	hr = newVisual->QueryInterface(IID_PPV_ARGS(this->_visual.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) throw Exception{ hr };

	// compose initialization.
	hr = this->_visual->SetContent(this->_swapchain.Get());
	if (FAILED(hr)) throw Exception{ hr };
	hr = this->_target->SetRoot(this->_visual.Get());
	if (FAILED(hr)) throw Exception{ hr };
	hr = this->_compositionDevice->Commit();
	if (FAILED(hr)) throw Exception{ hr };

	// depth buffer.
	{
		D3D11_TEXTURE2D_DESC d{};
		d.ArraySize = 1;
		d.BindFlags = D3D11_BIND_DEPTH_STENCIL;
		d.CPUAccessFlags = 0;
		d.Format = DXGI_FORMAT_D32_FLOAT;
		d.Height = w.Height();
		d.MipLevels = 1;
		d.MiscFlags = 0;
		d.SampleDesc.Count = 1;
		d.SampleDesc.Quality = 0;
		d.Usage = D3D11_USAGE_DEFAULT;
		d.Width = w.Width();
		hr = this->_device->CreateTexture2D(&d, nullptr, this->_depthBuffer.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw Exception{ hr };
	}

	// depth stencil view.
	{
		D3D11_DEPTH_STENCIL_VIEW_DESC d{};
		d.Format = DXGI_FORMAT_D32_FLOAT;
		d.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
		d.Texture2D.MipSlice = 0;
		hr = this->_device->CreateDepthStencilView(this->_depthBuffer.Get(), &d, this->_depthStencilView.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw Exception{ hr };
	}
}
ID3D11DeviceContext& DirectX11Manager::DeferredContext() noexcept {
	return *this->_deferredContext.Get();
}
/// # DESCRIPTION
/// 1. A simple getter function.
IDXGISwapChain4& DirectX11Manager::SwapChain() noexcept {
	return *this->_swapchain.Get();
}
ComPtr<IDXGISwapChain4>& DirectX11Manager::ComPtrSwapChain() noexcept {
	return this->_swapchain;
}
/// # DESCRIPTION
/// 1. A simple getter function.
ID3D11RenderTargetView& DirectX11Manager::RTV() noexcept {
	return *this->_rtv.Get();
}
/// # DESCRIPTION
/// 1. A simple getter function.
const D3D11_VIEWPORT& DirectX11Manager::Viewport() noexcept {
	return this->_viewport;
}
/// # DESCRIPTION
/// 1. A simple getter function.
IDCompositionDesktopDevice& DirectX11Manager::CompositionDevice() noexcept {
	return *this->_compositionDevice.Get();
}
/// # DESCRIPTION
/// 1. A simple getter function.
ID3D11DepthStencilView& DirectX11Manager::DepthStencilView() noexcept {
	return *this->_depthStencilView.Get();
}
/// # HELPER CONSTRUCTOR
/// 1. starting rendering jthread.
/// 2. static Factory.
/// 3. static Adapter.
/// 4. static Device & DeviceContext.
/// # REMARK
/// 1. Each static Getter will call this before trying to retrieve static member attributs.
void DirectX11Manager::InitializeStatics() {
	// prevent initialize multiple times.
	static std::mutex mutex;
	std::lock_guard locker(mutex);
	if (DirectX11Manager::_areStaticsInitialized) return;



	// factory.
	HRESULT hr;
	hr = CreateDXGIFactory2(0, IID_PPV_ARGS(DirectX11Manager::_factory.ReleaseAndGetAddressOf()));
	if (FAILED(hr)) throw Exception{ hr };

	// enum adapters, the first one.
	{
		auto& f = DirectX11Manager::_factory;
		hr = f->EnumAdapterByGpuPreference(0, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(DirectX11Manager::_adapter.ReleaseAndGetAddressOf()));
		if (FAILED(hr)) throw Exception{ hr };
	}

	// device and context.
	// My laptop only support D3D_FEATURE_LEVEL_12_1.
	{
		std::array fl = { D3D_FEATURE_LEVEL_12_2, D3D_FEATURE_LEVEL_12_1 };
		hr = D3D11CreateDevice(DirectX11Manager::_adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, NULL, NULL, fl.data(), static_cast<UINT>(fl.size()), D3D11_SDK_VERSION, DirectX11Manager::_device.ReleaseAndGetAddressOf(), NULL, DirectX11Manager::_immediateContext.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw Exception{ hr };
	}
	DirectX11Manager::_areStaticsInitialized = true;
}
/// # DESCRIPTION
/// 1. A simple getter function.
IDXGIFactory6& DirectX11Manager::Factory() noexcept {
	InitializeStatics();
	return *DirectX11Manager::_factory.Get();
}
/// # DESCRIPTION
/// 1. A simple getter function.
IDXGIAdapter4& DirectX11Manager::Adapter() noexcept {
	InitializeStatics();
	return *DirectX11Manager::_adapter.Get();
}
/// # DESCRIPTION
/// 1. A simple getter function.
ID3D11Device& DirectX11Manager::Device() noexcept {
	InitializeStatics();
	return *DirectX11Manager::_device.Get();
}
/// # DESCRIPTION
/// 1. A simple getter function.
ID3D11DeviceContext& DirectX11Manager::ImmediateContext() noexcept {
	InitializeStatics();
	return *DirectX11Manager::_immediateContext.Get();
}

