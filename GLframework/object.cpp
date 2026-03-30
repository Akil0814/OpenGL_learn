#include"object.h"

Object::Object()
{ }

Object::~Object()
{ }

void Object::set_position(glm::vec3 pos)
{
	_position = pos;
}

void Object::rotate_x(float angle)
{
	_angle_x += angle;
}

void Object::rotate_y(float angle)
{
	_angle_y += angle;
}

void Object::rotate_z(float angle)
{
	_angle_z += angle;
}

void Object::set_scale(glm::vec3 scale)
{
	_scale = scale;
}

glm::mat4 Object::get_model_matrix() const
{
	//缩放->旋转->平移
	glm::mat4 transform{ 1.0 };

	transform = glm::scale(transform, _scale);

	//pitch(点头)->yaw(摇头)->roll(滚)
	transform = glm::rotate(transform, glm::radians(_angle_x), glm::vec3(1.0f, 0.0f, 0.0f));//
	transform = glm::rotate(transform, glm::radians(_angle_y), glm::vec3(0.0f, 1.0f, 0.0f));//
	transform = glm::rotate(transform, glm::radians(_angle_z), glm::vec3(0.0f, 0.0f, 1.0f));//

	transform = glm::translate(glm::mat4(1.0f), _position) * transform;

	return transform;
}

glm::vec3 Object::get_position()const
{
	return _position;
}
