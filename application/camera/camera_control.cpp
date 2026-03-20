#include "camera_control.h"
#include"../../GLframework/core.h"
#include<iostream>


CameraControl::CameraControl()
{
}

void CameraControl::on_mouse(int button, int action, double x_pos, double y_pos)
{
	//判断是否按下
	if (action == GLFW_REPEAT)
		return;
	bool pressed = action == GLFW_PRESS ? true : false;

	//按下记录当前位置
	if (pressed)
	{
		_current_x = (float)x_pos;
		_current_y = (float)y_pos;
	}

	//根据按键不同 更改按键状态
	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
		_left_mouse_down = pressed;
		break;

	case GLFW_MOUSE_BUTTON_RIGHT:
		_right_mouse_down = pressed;
		break;

	case GLFW_MOUSE_BUTTON_MIDDLE:
		_middle_mouse_down = pressed;
		break;

	default:
		break;
	}
}

void CameraControl::on_key(int key, int action, int mods)
{
	bool pressed = action == GLFW_PRESS ? true : false;
	if (action == GLFW_REPEAT)
		return;
	_key_map[key] = pressed;
}