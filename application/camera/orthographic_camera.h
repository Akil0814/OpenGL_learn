#pragma once

#include "camera.h"

class OrthographicCamera :public Camera
{
public:
	OrthographicCamera(float left, float right, float top, float bottom, float near, float far);
	~OrthographicCamera();

	glm::mat4 get_projection_matrix() override;

private:
	float _left = { 0 };
	float _right = { 0 };
	float _top = { 0 };
	float _bottom = { 0 };
	float _near = { 0 };
	float _far = { 0 };

};