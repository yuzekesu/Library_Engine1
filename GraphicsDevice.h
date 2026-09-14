#pragma once
#include <d3d11.h>
#include <dxgi1_6.h>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

namespace Render {
	class GraphicsDevice {
	public:
	private:
		ComPtr<IDXGIFactory6> _factory;
		ComPtr<IDXGIAdapter4> _adapter;
		ComPtr<ID3D11Device> _device;
		ComPtr<ID3D11DeviceContext> _immediateContext;
	};
}

