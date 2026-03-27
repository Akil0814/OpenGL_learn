#pragma once
#include "../texture.h"
#include "material.h"

class PhongMaterial : public Material
{
public:
	PhongMaterial();
	~PhongMaterial();

public:
	Texture* _diffuse = { nullptr };
	float _shiness = { 1.0f };
};
