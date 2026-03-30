#pragma once
#include "light.h"
#include "../object.h"


class PointLight: public Light,public Object
{
public:
	PointLight();
	~PointLight();
private:
	float _k1 = 1.0f;
	float _k2 = 1.0f;
	float _kc = 1.0f;

};