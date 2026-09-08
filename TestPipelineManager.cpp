#pragma comment(lib, "d3dcompiler.lib")
#include "Exception.h"
#include "TestPipelineManager.h"
#include <Windows.h>
#include <d3dcompiler.h>
#include <numbers>
#include <ranges>
#include <stdexcept>
#include <string>
#include <string.h>

/// # INPUT ELEMENT DESC
/// 1. Nothing special.
D3D11_INPUT_ELEMENT_DESC InputElementDesc[]{
	{
			"POSITION", 0,
			DXGI_FORMAT_R32G32B32_FLOAT,
			0, 0,
			D3D11_INPUT_PER_VERTEX_DATA, 0
	}
};
/// # VERTEX SHADER
/// 1. The definition is at the botton of this file.
extern const std::string SHADER;
/// # CONSTRUCTOR
TestPipelineManager::TestPipelineManager(std::initializer_list<std::weak_ptr<Entity>> vector) : TestPipelineManager() {
	_pendingItems = vector;
}
/// # DESCRIPTION
/// 1. Render all stored drawables 
/// using DEFERRED device context, 
/// command list and global static
/// render queue in DirectX11Manager.
void TestPipelineManager::Run(IRenderWindow& w, DirectX11Manager& dx) {
	HRESULT hr;
	// clear rtv.
	const FLOAT c[] = { 0.f,0.f,0.f,0.0f };
	dx.DeferredContext().ClearRenderTargetView(&dx.RTV(), c);
	// topology & inputlayout
	dx.DeferredContext().IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	dx.DeferredContext().IASetInputLayout(this->_inputLayout.Get());
	// shaders
	dx.DeferredContext().VSSetShader(this->_vs.Get(), nullptr, 0);
	dx.DeferredContext().VSSetConstantBuffers(0, 1, this->_cb.GetAddressOf());
	dx.DeferredContext().PSSetConstantBuffers(0, 1, this->_cb.GetAddressOf());
	dx.DeferredContext().PSSetShader(this->_ps.Get(), nullptr, 0);
	// viewport
	dx.DeferredContext().RSSetViewports(1, &dx.Viewport());
	// rtv & dsv
	{
		const auto p = &dx.RTV();
		dx.DeferredContext().OMSetRenderTargets(1u, &p, &dx.DepthStencilView());
	}
	// rasterizer state
	dx.DeferredContext().RSSetState(this->_rs.Get());
	// vertex, index, constant buffers.
	const UINT offset = 0;
	const UINT stride = 3 * sizeof(float);
	const auto pw = this->_PerspectiveMatrix(w);
	const auto vw = this->_ViewMatrix();
	TestPipelineManager::ConstantBuffer cb; // constant buffer in struct.
	auto camera = this->_pendingItems.front().lock();
	if (!camera) return;
	DirectX::XMStoreFloat4(&cb.cameraAngle, camera->Position());
	std::erase_if(this->_pendingItems, [](const auto& item) {
		return item.expired();
		});
	// use the first pending item as the camera position.
	if (this->_pendingItems.size() == 0) return;
	auto perspectiveMatrix = this->_PerspectiveMatrix(w);
	for (auto& item : this->_pendingItems | std::views::drop(1)) {
		// weak ptr to share ptr.
		auto spEntity = item.lock();
		if (!spEntity) continue;
		auto& entity = *spEntity;
		// set vertex buffer and index buffer.
		dx.DeferredContext().IASetVertexBuffers(0, 1, entity.PPVertexID3D11Buffer(), &stride, &offset);
		dx.DeferredContext().IASetIndexBuffer(entity.PIndexID3D11Buffer(), DXGI_FORMAT_R32_UINT, 0);
		// cbuffer
		DirectX::XMStoreFloat4x4(&cb.worldMatrix, entity.WorldMatrix());
		// 🔞 WHY THE F*CK IS DIRECTXMATH USING ROW VECTOR BUT THE HLSL USING COLUMN VECTOR.
		// 🔞 WHY THE F*CK IS DIRECTXMATH USING ROW VECTOR BUT THE HLSL USING COLUMN VECTOR.
		// 🔞 WHY THE F*CK IS DIRECTXMATH USING ROW VECTOR BUT THE HLSL USING COLUMN VECTOR.
		// 🤓 Because the precedence of the multiplication operator in c++ is from Left to Right.
		// 🔞 WHY THE F*CK IS DIRECTXMATH USING ROW VECTOR BUT THE HLSL USING COLUMN VECTOR.
		// 🔞 WHY THE F*CK IS DIRECTXMATH USING ROW VECTOR BUT THE HLSL USING COLUMN VECTOR.
		// 🔞 WHY THE F*CK IS DIRECTXMATH USING ROW VECTOR BUT THE HLSL USING COLUMN VECTOR.
		DirectX::XMStoreFloat4x4(&cb.pvwMatrix, entity.WorldMatrix() * vw * pw);
		D3D11_MAPPED_SUBRESOURCE ms{};
		hr = dx.DeferredContext().Map(this->_cb.Get(), 0, D3D11_MAP_WRITE_DISCARD, 0, &ms);
		if (FAILED(hr)) throw Exception(hr);
		std::memcpy(ms.pData, &cb, sizeof(cb));
		dx.DeferredContext().Unmap(this->_cb.Get(), 0);
		// draw
		dx.DeferredContext().DrawIndexed(static_cast<UINT>(entity.Indices().size()), 0, 0);
	}
	// send command list.
	ComPtr<ID3D11CommandList> cl;
	hr = dx.DeferredContext().FinishCommandList(FALSE, cl.ReleaseAndGetAddressOf());
	DirectX11Manager::PushCommandListToTheRenderQueue(cl, dx.ComPtrSwapChain());
}
TestPipelineManager& TestPipelineManager::Instance() {
	static auto global_pipeline = std::make_unique<TestPipelineManager>();
	return *global_pipeline;
}
void TestPipelineManager::Add(std::weak_ptr<Entity> entity) {
	static std::vector<std::weak_ptr<Entity>> global_vector;
	global_vector.push_back(entity);
}
/// # CONSTRUCTOR
/// 1. Vertex shader.
/// 2. Input layout (checks with bytecode of the vertex shader).
/// 3. Pixel shader.
TestPipelineManager::TestPipelineManager() {
	HRESULT hr;
	auto& dx = DirectX11Manager::Device();
	ComPtr<ID3DBlob> byteCode;
	ComPtr<ID3DBlob> errorMsg;
	// vertex shader.
	hr = D3DCompile(SHADER.c_str(), SHADER.size(), "my embedded vertex shader", nullptr, nullptr, "vMain", "vs_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, byteCode.ReleaseAndGetAddressOf(), errorMsg.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(*errorMsg.Get());
	hr = dx.CreateVertexShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, this->_vs.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(hr);
	// input layout.
	dx.CreateInputLayout(InputElementDesc, std::size(InputElementDesc), byteCode->GetBufferPointer(), byteCode->GetBufferSize(), this->_inputLayout.ReleaseAndGetAddressOf());
	// pixel shader.
	hr = D3DCompile(SHADER.c_str(), SHADER.size(), "my embedded pixel shader", nullptr, nullptr, "pMain", "ps_5_0", D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, 0, byteCode.ReleaseAndGetAddressOf(), errorMsg.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(*errorMsg.Get());
	hr = dx.CreatePixelShader(byteCode->GetBufferPointer(), byteCode->GetBufferSize(), nullptr, this->_ps.ReleaseAndGetAddressOf());
	if (FAILED(hr)) throw Exception(hr);
	// constant buffer.
	{
		D3D11_BUFFER_DESC d{};
		d.ByteWidth = sizeof(TestPipelineManager::ConstantBuffer);
		d.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		d.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		d.Usage = D3D11_USAGE_DYNAMIC;
		hr = dx.CreateBuffer(&d, nullptr, this->_cb.ReleaseAndGetAddressOf());
		if (FAILED(hr)) throw Exception(hr);
	}
	// rasterizer state.
	{
		D3D11_RASTERIZER_DESC d{};
		d.FillMode = D3D11_FILL_SOLID;
		d.CullMode = D3D11_CULL_BACK;
		d.DepthClipEnable = TRUE;
		dx.CreateRasterizerState(&d, this->_rs.ReleaseAndGetAddressOf());
	}

}
/// # DESCRIPTION
/// 1. Create the perspective matrix based on the window size.
/// 2. Else are just preset, like FOV, and the near/far Z.
DirectX::XMMATRIX TestPipelineManager::_PerspectiveMatrix(IRenderWindow& w) const noexcept {
	// perspective.
	float width = static_cast<float>(w.Width());
	float height = static_cast<float>(w.Height());
	return DirectX::XMMatrixPerspectiveFovLH(std::numbers::pi_v<float> / 2.f, width / height, 0.1f, 1000.f);
}
/// # DESCRIPTION
/// 1. Uses the first pending item as the camera position to create the view matrix.
DirectX::XMMATRIX TestPipelineManager::_ViewMatrix() const {
	//view.
	if (this->_pendingItems.size() == 0) throw std::runtime_error("Empty _pendingItems, unable to get camera information in the TestPipelineManager.");
	auto spEntity = this->_pendingItems.front().lock();
	if (!spEntity) throw std::runtime_error("Invalid weak ptr, unable to get camera information in the TestPipelineManager.");
	auto& movable = *spEntity;
	return DirectX::XMMatrixLookAtLH(movable.Position(), DirectX::XMVectorAdd(movable.Position(), movable.Angle(IMovable::Direction::FORWARD)), movable.Angle(IMovable::Direction::UP));
}
/// # VERTEX SHADER
const std::string SHADER = R"HLSL(
	cbuffer ConstantBuffer : register(b0) {
    float4x4 pvwMatrix;
    float4x4 worldMatrix;
    float4 cameraPosition; // xyz = camera position in world space
	};

	struct VertexInput {
		float3 position : POSITION;
	};

	struct PixelInput {
		float4 position      : SV_POSITION;
		float3 worldPosition : POSITION0;
	};

	PixelInput vMain(VertexInput input) {
		PixelInput output;

		float4 localPosition = float4(input.position, 1.0f);
		float4 worldPosition = mul(worldMatrix, localPosition);

		output.position      = mul(pvwMatrix, localPosition);
		output.worldPosition = worldPosition.xyz;

		return output;
	}

	float4 pMain(PixelInput input, bool isFrontFace : SV_IsFrontFace) : SV_Target {
		float3 positionDx = ddx(input.worldPosition);
		float3 positionDy = ddy(input.worldPosition);
		float3 normal = normalize(cross(positionDy, positionDx));

		if (isFrontFace) normal = -normal;

		// Direction from the surface toward the camera.
		float3 lightDirection = normalize(cameraPosition.xyz - input.worldPosition);

		float diffuseStrength =	saturate(dot(normal, lightDirection));

		float3 objectColor = float3(1.f, 1.f, 1.f);
		float ambient = 0.1f;

		float3 finalColor =	max(objectColor * diffuseStrength, float3(0.1f, 0.1f, 0.1f));
		
		float distanceFromCamera = length(cameraPosition.xyz - input.worldPosition);
		distanceFromCamera *= 1.f;
		float alpha = saturate(2.f / distanceFromCamera / distanceFromCamera);
		return float4(finalColor * alpha, alpha);
	}
)HLSL";