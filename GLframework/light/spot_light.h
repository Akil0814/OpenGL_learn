#pragma once
#include "light.h"
#include "../object.h"


class SpotLight : public Light, public Object
{
public:
	SpotLight();
	~SpotLight();

public:
	glm::vec3 _target_direction = { 1.0f,0.0f,0.0f };
	float _inner_angle = { 0.0f };
	float _outer_angle = { 0.0f };
};