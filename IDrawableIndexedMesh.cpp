#pragma comment(lib, "d3d11.lib")
#include "DirectX11Manager.h"
#include "Exception.h"
#include "IDrawableIndexedMesh.h"
#include <format>
#include <fstream>
#include <string>
#include <vector>

/// # CONSTRUCTOR
/// 1. Load from the disk.
/// 2. Cache the datas and create ID3D11Buffer for them.
IDrawableIndexedMesh::IDrawableIndexedMesh(std::wstring fileName) {
	// add the extention name if not yet.
	if (fileName.find(L".im") == std::wstring::npos) fileName += L".im";
	std::ifstream f(fileName, std::fstream::binary);
	if (f.fail()) throw Exception(std::format(L"The {} file doesnt found", fileName));
	struct Header {
		uint64_t nV, nI;
	} h;
	f.read(reinterpret_cast<char*>(&h), sizeof(h));
	this->_v.resize(h.nV * 3);
	this->_i.resize(h.nI);
	f.read(reinterpret_cast<char*>(this->_v.data()), h.nV * sizeof(decltype(this->_v)::value_type) * 3);
	f.read(reinterpret_cast<char*>(this->_i.data()), h.nI * sizeof(decltype(this->_i)::value_type));
	UpdateD3D11Buffer(); // the ID3DBuffer part.
}
/// # DESCRIPTION
/// 1. Updates the vertex- & the index buffer.
void IDrawableIndexedMesh::UpdateD3D11Buffer() {
	// local variables.
	// 1. reusing some of the DESC, because they sharing the same value.
	HRESULT hr;
	auto& device = DirectX11Manager::Device();
	D3D11_BUFFER_DESC vd{}, id{};
	vd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	id.BindFlags = D3D11_BIND_INDEX_BUFFER;
	vd.CPUAccessFlags = id.CPUAccessFlags = 0;
	vd.MiscFlags = id.MiscFlags = 0;
	vd.StructureByteStride = id.StructureByteStride = 0;
	vd.Usage = id.Usage = D3D11_USAGE_DEFAULT;

	// set the vertex ID3DBuffer.
	vd.ByteWidth = static_cast<UINT>(sizeof(decltype(this->_v)::value_type) * this->_v.size());
	D3D11_SUBRESOURCE_DATA sd{};
	sd.pSysMem = this->_v.data();
	hr = device.CreateBuffer(&vd, &sd, this->_vb.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(hr);

	// set the index ID3DBuffer.
	id.ByteWidth = static_cast <UINT>(sizeof(decltype(this->_i)::value_type) * this->_i.size());
	sd.pSysMem = this->_i.data();
	hr = device.CreateBuffer(&id, &sd, this->_ib.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(hr);
}
/// # DESCRIPTION
/// 1. A simple getter function.
std::vector<float>& IDrawableIndexedMesh::Vertices() noexcept {
	return this->_v;
}
/// # DESCRIPTION
/// 1. A simple getter function.
std::vector<uint32_t>& IDrawableIndexedMesh::Indices() noexcept {
	return this->_i;
}
/// # DESCRIPTION
/// 1. A simple getter function.
ID3D11Buffer** IDrawableIndexedMesh::PPVertexID3D11Buffer() noexcept {
	return this->_vb.GetAddressOf();
}
/// # DESCRIPTION
/// 1. A simple getter function.
ID3D11Buffer* IDrawableIndexedMesh::PIndexID3D11Buffer() noexcept {
	return this->_ib.Get();
}

