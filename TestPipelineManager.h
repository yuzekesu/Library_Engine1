#pragma once
#include "DirectX11Manager.h"
#include "Entity.h"
#include "IDrawableIndexedMesh.h"
#include "IMovable.h"
#include "IRenderPipeline.h"
#include "IRenderWindow.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include <memory>
#include <vector>
#include <wrl/client.h>
using Microsoft::WRL::ComPtr;

class TestPipelineManager : public IRenderPipeline {
public:
	struct PendingItem {
		std::weak_ptr<IMovable> wpMovable;
		std::weak_ptr<IDrawableIndexedMesh> wpDrawable;
	};
	TestPipelineManager(std::initializer_list<std::weak_ptr<Entity>>);
	void Add(std::weak_ptr<Entity>);
	void Run(IRenderWindow&, DirectX11Manager&) override;
	static TestPipelineManager& Instance();
	static void Add(std::weak_ptr<Entity>);
protected:
	TestPipelineManager();
private:
	struct ConstantBuffer {
		DirectX::XMFLOAT4X4 pvwMatrix;
		DirectX::XMFLOAT4X4 worldMatrix;
		DirectX::XMFLOAT4 cameraAngle;
	};
	ComPtr<ID3D11InputLayout> _inputLayout;
	ComPtr<ID3D11VertexShader> _vs;
	ComPtr<ID3D11PixelShader> _ps;
	ComPtr<ID3D11Buffer> _cb;
	ComPtr<ID3D11RasterizerState> _rs;
	std::vector<std::weak_ptr<Entity>> _pendingItems;
	DirectX::XMMATRIX _PerspectiveMatrix(IRenderWindow&) const noexcept;
	DirectX::XMMATRIX _ViewMatrix() const;
};

