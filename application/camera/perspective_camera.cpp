#include "perspective_camera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far, glm::vec3 position) :
	_fovy(fovy), _aspect(aspect), _near(near), _far(far), Camera(position) {}


glm::mat4 PerspectiveCamera::get_projection_matrix()
{
	return glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
}

void PerspectiveCamera::scale(float delta_scale)
{
	glm::vec3 front = glm::cross(_up, _right);
	_position += (front * delta_scale);
}
