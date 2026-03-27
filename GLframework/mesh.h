#pragma once
#include "object.h"
#include "geometry.h"
#include "material/material.h"

class Mesh :public Object
{
public:
	Mesh(Geometry* geometry, Material* material);
	~Mesh();

public:
	Geometry* _geometry = { nullptr };
	Material* _material = { nullptr };
};