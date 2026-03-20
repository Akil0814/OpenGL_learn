#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom, float near, float far, glm::vec3 position):
	_left(left), _right(right), _top(top), _bottom(bottom), _near(near), _far(far), Camera(position) {}


glm::mat4 OrthographicCamera::get_projection_matrix()
{
	float scale = std::pow(2.0f, _scale);
	return glm::ortho(_left*scale, _right*scale, _bottom*scale, _top*scale, _near, _far);
}

void OrthographicCamera::scale(float delta_scale)
{
	_scale += delta_scale;
}