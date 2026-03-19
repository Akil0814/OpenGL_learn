#include "track_ball_camera_control.h"

TrackBallCameraControl::TrackBallCameraControl()
{

}
TrackBallCameraControl::~TrackBallCameraControl()
{

}


void TrackBallCameraControl::on_cursor(double x_pos, double y_pos)
{
	if (_left_mouse_down)
	{
		//调整相机各类参数
		float delta_x = (x_pos - _current_x) * _sensitivity;
		float delta_y = (y_pos - _current_y) * _sensitivity;

		//分开pitch跟yaw各自计算
		//pitch(delta_y);
		//yaw(delta_x);
		pitch(-delta_y);
		yaw(-delta_x);
	}
	else if (_middle_mouse_down)
	{
		float delta_x = (x_pos - _current_x) * _move_speed;
		float delta_y = (y_pos - _current_y) * _move_speed;

		_camera->_position += _camera->_up * delta_y;
		_camera->_position -= _camera->_right * delta_x;
	}

	_current_x = x_pos;
	_current_y = y_pos;
}

void TrackBallCameraControl::on_scroll(float offset)
{
	_camera->scale(_scale_speed*offset);
}

void TrackBallCameraControl::pitch(float angle)
{
	//绕着_right旋转
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), _camera->_right);

	//影响当前相机的up向量和位置
	_camera->_up = mat * glm::vec4(_camera->_up, 0.0f);
	_camera->_position = mat * glm::vec4(_camera->_position, 1.0f);
}

void TrackBallCameraControl::yaw(float angle)
{
	//绕着世界坐标的y轴旋转
	glm::mat4 mat = glm::rotate(glm::mat4(1.0f), glm::radians(angle), glm::vec3(0.0f,1.0f,0.0f));
	_camera->_up = mat * glm::vec4(_camera->_up, 0.0f);
	_camera->_right = mat * glm::vec4(_camera->_right, 0.0f);
	_camera->_position = mat * glm::vec4(_camera->_position, 0.0f);

}

