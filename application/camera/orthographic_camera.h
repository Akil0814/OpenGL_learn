#pragma once

#include "camera.h"

class OrthographicCamera :public Camera
{
public:
	OrthographicCamera(float left, float right, float top, float bottom, float near, float far, glm::vec3 position = { 0.0f,0.0f,3.0f });
	~OrthographicCamera()=default;

	glm::mat4 get_projection_matrix() override;

	void scale(float delta_scale) override;

private:
	float _left = { 0.0f };
	float _right = { 0.0f };
	float _top = { 0.0f };
	float _bottom = { 0.0f };
	float _near = { 0.0f };
	float _far = { 0.0f };

	float _scale = { 0.0f };
};