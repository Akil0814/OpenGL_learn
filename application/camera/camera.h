#pragma once
#include"../../GLframework/core.h"

class Camera
{
public:
	Camera();
	~Camera();

	virtual void scale(float delta_scale);

	glm::mat4 get_view_matrix();
	virtual glm::mat4 get_projection_matrix();

public:
	glm::vec3 _position{ 0.0f,0.0f,3.0f };
	glm::vec3 _up{ 0.0f,1.0f,0.0f };
	glm::vec3 _right{ 1.0f,0.0f,0.0f };
};