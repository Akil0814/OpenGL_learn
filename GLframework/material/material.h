#pragma once
#include "../core.h"
#include "../object.h"

enum class MaterialType
{
	PhongMaterial,
	WhiteMaterial
};

class Material
{
public:
	Material();
	~Material();
public:
	MaterialType _type;

	bool _depth_test = { true };
	GLenum _depth_func = { GL_LESS };
	bool _depth_write = { true };
};