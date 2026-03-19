#pragma once

//GLM
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 get_view_matrix();
	virtual glm::mat4 get_projection_matrix();

public:
	glm::vec3 _position{ 0.0f,0.0f,1.0f };
	glm::vec3 _up{ 0.0f,1.0f,0.0f };
	glm::vec3 _right{ 1.0f,0.0f,0.0f };
};