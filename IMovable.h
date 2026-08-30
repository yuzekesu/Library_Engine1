#pragma once
#include <DirectXMath.h>

class IMovable {
public:
	virtual ~IMovable() = default;
	enum Direction {
		UP,
		DOWN,
		FORWARD,
		BACK,
		LEFT,
		RIGHT
	};
	void Rotate(float pitchDegrees, float yawDegrees, float rollDegrees) noexcept;
	void Turn(float pitchDegrees, float yawDegrees, float rollDegrees) noexcept;
	void Move(IMovable::Direction, float) noexcept;
	DirectX::XMVECTOR Angle(IMovable::Direction) const;
	DirectX::XMVECTOR Position()const noexcept;
	DirectX::XMMATRIX WorldMatrix() const noexcept;
private:
	unsigned _rotationCounter = 0;
	DirectX::XMVECTOR _rotation = DirectX::XMQuaternionIdentity();
	DirectX::XMVECTOR _position = DirectX::XMVectorSet(0.f, 0.f, 0.f, 0.f);
};

