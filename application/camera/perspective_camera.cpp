#include "perspective_camera.h"

PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far):
	_fovy(fovy),_aspect(aspect),_near(near),_far(far)
{

}

PerspectiveCamera::~PerspectiveCamera()
{

}

glm::mat4 PerspectiveCamera::get_projection_matrix()
{
	return glm::perspective(glm::radians(_fovy), _aspect, _near, _far);
}