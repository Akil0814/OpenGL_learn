#include "tools.h"

void Tools::decompose(glm::mat4 matrix, glm::vec3& position,
	glm::vec3& Euler_angle, glm::vec3& scale)
{
	//四元数 用来表示旋转变换
	glm::quat quaternion;
	glm::vec3 skew;
	glm::vec4 perspective;

	glm::decompose(matrix, scale, quaternion, position, skew, perspective);

	//四元数 -> 欧拉角
	glm::mat4 rotation = glm::toMat4(quaternion);
	glm::extractEulerAngleXYZ(rotation, Euler_angle.x, Euler_angle.y, Euler_angle.z);

	//结构出来的角度是弧度，转化为角度
	Euler_angle.x = glm::degrees(Euler_angle.x);
	Euler_angle.y = glm::degrees(Euler_angle.y);
	Euler_angle.z = glm::degrees(Euler_angle.z);
}