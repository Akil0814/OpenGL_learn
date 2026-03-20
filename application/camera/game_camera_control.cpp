#include "game_camera_control.h"

GameCameraControl::GameCameraControl()
{

}

void GameCameraControl::on_cursor(double x_pos, double y_pos)
{
	float delta_x = (x_pos - _current_x) * _sensitivity;
	float delta_y = (y_pos - _current_y) * _sensitivity;

	if (_left_mouse_down)
	{
		pitch(-delta_y);
		yaw(-delta_x);
	}

	_current_x = x_pos;
	_current_y = y_pos;
}

void GameCameraControl::pitch(float angle)
{
	_pitch += angle;
	if (_pitch > 89.0f || _pitch < -89.0f)
	{
		_pitch -= angle;
		return;
	}

	//pitch不会影响_position
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), _camera->_right);
	_camera->_up = mat * glm::vec4(_camera->_up, 0.0f);
}

void GameCameraControl::yaw(float angle)
{
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	_camera->_up = mat * glm::vec4(_camera->_up, 0.0f);
	_camera->_right = mat * glm::vec4(_camera->_right, 0.0f);
}

void GameCameraControl::on_update()
{
	glm::vec3 direction(0.0f);

	auto front = glm::cross(_camera->_up, _camera->_right);
	auto right = _camera->_right;

	if (_key_map[GLFW_KEY_W])
		direction += front;

	if (_key_map[GLFW_KEY_S])
		direction -= front;

	if (_key_map[GLFW_KEY_A])
		direction -= right;
	
	if (_key_map[GLFW_KEY_D])
		direction += right;

	//此时direction有可能不为1的长度，也有可能是0的长度
	if (glm::length(direction) != 0)
	{
		direction = glm::normalize(direction);
		_camera->_position += direction * _speed;
	}
}