#include "orthographic_camera.h"

OrthographicCamera::OrthographicCamera(float left, float right, float top, float bottom, float near, float far):
	_left(left), _right(right), _top(top), _bottom(bottom), _near(near), _far(far)
{
}

OrthographicCamera::~OrthographicCamera()
{

}

glm::mat4 OrthographicCamera::get_projection_matrix()
{
	return glm::ortho(_left, _right, _bottom, _top, _near, _far);
}