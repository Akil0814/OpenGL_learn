#include "camera.h"

//可以加入初始化参数
Camera::Camera()
{

}

Camera::~Camera()
{

}

glm::mat4 Camera::get_view_matrix()
{
	// lookAt
	// eye： 相机位置
	// center:看向世界坐标的那个点
	// top: 穹顶 (使用_up替代)
	glm::vec3 front = glm::cross(_up, _right);
	glm::vec3 center = _position + front;

	return glm::lookAt(_position, center, _up);
}