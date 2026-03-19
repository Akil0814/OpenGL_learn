#pragma once

#include "camera.h"

#include <map>

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	virtual void on_mouse(int button, int action, double x_pos, double y_pos);
	virtual void on_cursor(double x_pos, double y_pos);
	virtual void on_key(int key, int action, int mods);
	virtual void on_scroll(float offset);


	virtual void on_update();

	void set_camera(Camera* camera) { _camera = camera; }
	void set_sensitivity(float s) { _sensitivity = s; }
	void set_scale_speed(float s) { _scale_speed = s; }


protected:
	//鼠标按键状态
	bool _left_mouse_down = false;
	bool _right_mouse_down = false;
	bool _middle_mouse_down = false;

	//当前鼠标位置
	float _current_x = { 0.0f };
	float _current_y = { 0.0f };

	//敏感度
	float _sensitivity = { 0.2f };

	//键盘相关按键状态
	std::map<int, bool> _key_map;

	//当前控制的摄像机
	Camera* _camera = nullptr;

	//记录相机缩放速度
	float _scale_speed = { 0.2f };

};