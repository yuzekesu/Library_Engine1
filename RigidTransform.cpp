#include "RigidTransform.h"
#include <stdexcept>
inline const unsigned MAXIMUM_ROTAION_COUNT = 300;
using namespace DirectX;
inline const XMVECTOR DEFAULT_ANGLE_UP = XMVectorSet(0.f, 1.f, 0.f, 0.f);
inline const XMVECTOR DEFAULT_ANGLE_DOWN = XMVectorSet(0.f, -1.f, 0.f, 0.f);
inline const XMVECTOR DEFAULT_ANGLE_FORWARD = XMVectorSet(0.f, 0.f, 1.f, 0.f);
inline const XMVECTOR DEFAULT_ANGLE_BACK = XMVectorSet(0.f, 0.f, -1.f, 0.f);
inline const XMVECTOR DEFAULT_ANGLE_LEFT = XMVectorSet(-1.f, 0.f, 0.f, 0.f);
inline const XMVECTOR DEFAULT_ANGLE_RIGHT = XMVectorSet(1.f, 0.f, 0.f, 0.f);
/// # DESCRIPTION	
/// 1. Move in 8-directions depending on the current facing angle.
/// # REMARK
/// 1. The position will be stored as an XMVECTOR.
void RigidTransform::Move(RigidTransform::Direction direction, float distance) noexcept {
	this->_position += Angle(direction) * distance;
}
/// # DESCRIPTION 
/// 1. Rotate with quaternion.
/// # REMARK
/// 1. The rotation will be stored as an XMVECTOR.
void RigidTransform::Rotate(float pitchDegrees, float yawDegrees, float rollDegrees) noexcept {
	//const float pitch = XMConvertToRadians(pitchDegrees);
	//const float yaw = XMConvertToRadians(yawDegrees);
	//const float roll = XMConvertToRadians(rollDegrees);
	//const XMVECTOR delta = XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
	//this->_rotation = XMQuaternionMultiply(this->_rotation, delta);
	//if (this->_rotationCounter++ > MAXIMUM_ROTAION_COUNT) {
	//	this->_rotation = XMQuaternionNormalize(this->_rotation);
	//	this->_rotationCounter = 0;
	//}

	const float pitch = XMConvertToRadians(pitchDegrees);
	const float yaw = XMConvertToRadians(yawDegrees);
	const float roll = XMConvertToRadians(rollDegrees);
	const XMVECTOR worldUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	const XMVECTOR worldRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	const XMVECTOR worldForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	const XMVECTOR yawRotation = XMQuaternionRotationAxis(worldUp, yaw);
	const XMVECTOR pitchRotation = XMQuaternionRotationAxis(worldRight, pitch);
	const XMVECTOR rollRotation = XMQuaternionRotationAxis(worldForward, roll);
	// World-space yaw: current orientation followed by world yaw.
	_rotation = XMQuaternionMultiply(yawRotation, _rotation);
	// Local-space pitch: local rotation followed by current orientation.
	_rotation = XMQuaternionMultiply(pitchRotation, _rotation);
	// Local-space roll.
	_rotation = XMQuaternionMultiply(rollRotation, _rotation);
	_rotation = XMQuaternionNormalize(_rotation);
}
/// # DESCRIPTION 
/// 1. Rotate with quaternion.
/// # REMARK
/// 1. The rotation will be stored as an XMVECTOR.
void RigidTransform::Turn(float pitchDegrees, float yawDegrees, float rollDegrees) noexcept {
	const float pitch = XMConvertToRadians(pitchDegrees);
	const float yaw = XMConvertToRadians(yawDegrees);
	const float roll = XMConvertToRadians(rollDegrees);
	const XMVECTOR localUp = XMVectorSet(0.f, 1.f, 0.f, 0.f);
	const XMVECTOR localRight = XMVectorSet(1.f, 0.f, 0.f, 0.f);
	const XMVECTOR localForward = XMVectorSet(0.f, 0.f, 1.f, 0.f);
	const XMVECTOR yawRotation = XMQuaternionRotationAxis(localUp, yaw);
	const XMVECTOR pitchRotation = XMQuaternionRotationAxis(localRight, pitch);
	const XMVECTOR rollRotation = XMQuaternionRotationAxis(localForward, roll);
	// World-space yaw: current orientation followed by world yaw.
	_rotation = XMQuaternionMultiply(yawRotation, _rotation);
	// Local-space pitch: local rotation followed by current orientation.
	_rotation = XMQuaternionMultiply(pitchRotation, _rotation);
	// Local-space roll.
	_rotation = XMQuaternionMultiply(rollRotation, _rotation);
	_rotation = XMQuaternionNormalize(_rotation);
}
/// # DESCRIPTION
/// 1. Return the XMVECTOR that represent the normalized vector of the direction.
/// # REMARK
/// 1. The return value is NOT quaternion.
XMVECTOR RigidTransform::Angle(RigidTransform::Direction direction) const {
	XMVECTOR result;
	switch (direction) {
	case RigidTransform::Direction::BACK: result = XMVector3Rotate(DEFAULT_ANGLE_BACK, this->_rotation); break;
	case RigidTransform::Direction::DOWN: result = XMVector3Rotate(DEFAULT_ANGLE_DOWN, this->_rotation); break;
	case RigidTransform::Direction::FORWARD: result = XMVector3Rotate(DEFAULT_ANGLE_FORWARD, this->_rotation); break;
	case RigidTransform::Direction::LEFT: result = XMVector3Rotate(DEFAULT_ANGLE_LEFT, this->_rotation); break;
	case RigidTransform::Direction::RIGHT: result = XMVector3Rotate(DEFAULT_ANGLE_RIGHT, this->_rotation); break;
	case RigidTransform::Direction::UP: result = XMVector3Rotate(DEFAULT_ANGLE_UP, this->_rotation); break;
	default:
		throw std::invalid_argument("The specified direction is not recognizable in the enumuration IMovable::Direction.");
	}
	return result;
}
/// # DESCRIPTION
/// 1. A simple getter function.
DirectX::XMVECTOR RigidTransform::Position() const noexcept {
	return this->_position;
}
/// # DESCRIPTION
/// 1. Combine the rotation and the position into one matrix.
/// # REMARK
/// 1. Row vector in DirectXMath.
XMMATRIX RigidTransform::WorldMatrix() const noexcept {
	XMMATRIX rm = XMMatrixRotationQuaternion(this->_rotation);
	XMMATRIX wm = XMMatrixTranslationFromVector(this->_position);
	return rm * wm;
}
