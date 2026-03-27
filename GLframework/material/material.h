#pragma once
#include "../core.h"
#include "../object.h"

enum class MaterialType
{
	PhongMaterial,
};

class Material
{
public:
	Material();
	~Material();
public:
	MaterialType _type;
};