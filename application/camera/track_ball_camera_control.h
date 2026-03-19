#pragma once
#include "camera_control.h"

class TrackBallCameraControl : public CameraControl
{
public:
	TrackBallCameraControl();
	~TrackBallCameraControl();

	void on_cursor(double x_pos, double y_pos) override;
	void on_scroll(float offset) override;


private:
	void pitch(float angle);
	void yaw(float angle);

private:
	float _move_speed = { 0.005f };
};