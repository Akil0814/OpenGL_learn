#pragma once
#include "../core.h"

class Light
{
public:
	Light();
	~Light();

public:
	glm::vec3 _color{ 1.0f };
	float _specular_intensity = { 1.0f };

};