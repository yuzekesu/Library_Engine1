#include "Entity.h"
#include "IDrawableIndexedMesh.h"
#include <string>

/// # CONSTRUCTION
/// 1. Category name.
/// 2. Interface.
Entity::Entity(std::wstring category) : _category{ category }, IDrawableIndexedMesh{ category } {}
/// # DESCRIPTION
/// 1. A simple getter function.
std::wstring_view Entity::Category() {
	return this->_category;
}
