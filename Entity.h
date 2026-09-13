#pragma once
#include "IDrawableIndexedMesh.h"
#include "RigidTransform.h"
#include <memory>
#include <string>
#include <string_view>

class Entity :
	public IDrawableIndexedMesh {
public:
	Entity() = delete;
	Entity(std::wstring);
	std::wstring_view Category();
	RigidTransform& Transform();
private:
	std::wstring _category;
	std::shared_ptr<RigidTransform> _pTransform = std::make_shared<RigidTransform>();
};

