#pragma once
#include <cstdint>
#include <d3d11.h>
#include <string>
#include <vector>
#include <wrl.h>
using Microsoft::WRL::ComPtr;

/// # DESCRIPTION
/// 1. This class will be used as a interface
/// for any Entity that is drawable in d3d.
class IDrawableIndexedMesh {
public:
	virtual ~IDrawableIndexedMesh() = default;
	IDrawableIndexedMesh() = delete;
	IDrawableIndexedMesh(std::wstring);
	void UpdateD3D11Buffer();
	std::vector<float>& Vertices() noexcept;
	std::vector<uint32_t>& Indices() noexcept;
	ID3D11Buffer** PPVertexID3D11Buffer() noexcept;
	ID3D11Buffer* PIndexID3D11Buffer() noexcept;
private:
	std::vector<float> _v;
	std::vector<uint32_t> _i;
	ComPtr<ID3D11Buffer> _vb;
	ComPtr<ID3D11Buffer> _ib;
};

