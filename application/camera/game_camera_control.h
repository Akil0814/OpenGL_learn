#pragma once
#include "camera_control.h"

class GameCameraControl :public CameraControl
{
public:
	GameCameraControl();
	~GameCameraControl();

	void on_cursor(double x_pos, double y_pos) override;
	void on_update() override;

	void set_speed(float s) { _speed = s; }

private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float _pitch = { 0.0f };
	float _speed = { 0.01f };
};