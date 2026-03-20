#pragma once
#include "camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(float fovy, float aspect,float near,float far, glm::vec3 position = { 0.0f,0.0f,3.0f });
	~PerspectiveCamera()=default;

	glm::mat4 get_projection_matrix() override;

	void scale(float delta_scale) override;

private:
	float _fovy = { 0 };
	float _aspect = { 0 };
	float _near = { 0 };
	float _far = { 0 };
};

