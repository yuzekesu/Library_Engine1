#pragma once
#include "IDrawableIndexedMesh.h"
#include "IMovable.h"
#include <string>
#include <string_view>

class Entity :
	public IMovable,
	public IDrawableIndexedMesh {
public:
	Entity() = delete;
	Entity(std::wstring);
	std::wstring_view Category();
private:
	std::wstring _category;
};

