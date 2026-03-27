#pragma once
#include "light.h"

class DirectionalLight : public Light
{
public:
	DirectionalLight();
	~DirectionalLight();

public:
	glm::vec3 _direction{ -1.0f };
};